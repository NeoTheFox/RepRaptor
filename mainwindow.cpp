#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->fileBox->setDisabled(true);
    ui->sendBtn->setDisabled(true);
    ui->pauseBtn->setDisabled(true);
    ui->progressBar->setValue(0);
    ui->controlBox->setDisabled(true);
    ui->consoleGroup->setDisabled(true);
    ui->pauseBtn->setDisabled("true");

    ui->baudbox->addItem(QString::number(4800));
    ui->baudbox->addItem(QString::number(9600));
    ui->baudbox->addItem(QString::number(115200));
    ui->baudbox->addItem(QString::number(128000));
    ui->baudbox->addItem(QString::number(230400));
    ui->baudbox->addItem(QString::number(250000));
    ui->baudbox->addItem(QString::number(460800));
    ui->baudbox->addItem(QString::number(500000));
    if(settings.value("printer/baudrateIndex").toInt()) ui->baudbox->setCurrentIndex(settings.value("printer/baudrateIndex").toInt());
    ui->baudbox->setCurrentIndex(2);

    ui->extruderlcd->setPalette(Qt::red);
    ui->bedlcd->setPalette(Qt::red);

    if(!settings.value("core/firstrun").toBool()) firstrun = true;

    checkingTemperature = settings.value("core/checktemperature").toBool();
    ui->checktemp->setChecked(checkingTemperature);

    if(!firstrun) echo = settings.value("core/echo").toBool();
    else echo = false;

    sending = false;
    paused = false;
    injectingCommand = false;
    readingFiles = false;
    sdprinting = false;
    sdBytes = 0;
    userCommand = "";
    currentLine = 0;
    readyRecieve = 0;

    serialupdate();

    connect(&printer, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));
    connect(&printer, SIGNAL(readyRead()), this, SLOT(readSerial()));
    connect(&statusTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
    connect(&sendTimer, SIGNAL(timeout()), this, SLOT(sendNext()));
    connect(&statusWatcher, SIGNAL(finished()), this, SLOT(updateStatus()));
    connect(&sdWatcher, SIGNAL(finished()), this, SLOT(updateSDStatus()));
    connect(this, SIGNAL(sdReady()), this, SLOT(initSDprinting()));
    connect(&progressSDTimer, SIGNAL(timeout()), this, SLOT(checkSDStatus()));

    if(settings.value("core/statusinterval").toInt()) statusTimer.setInterval(settings.value("core/statusinterval").toInt());
    else statusTimer.setInterval(3000);
    statusTimer.start();

    if(settings.value("core/senderinterval").toInt()) sendTimer.setInterval(settings.value("core/senderinterval").toInt());
    else sendTimer.setInterval(5);
    sendTimer.start();

    progressSDTimer.setInterval(1500);
    progressSDTimer.start();

    tempWarning.setInterval(10000);

    sinceLastTemp.start();
}

MainWindow::~MainWindow()
{
    if(gfile.isOpen()) gfile.close();
    if(printer.isOpen()) printer.close();

    settings.setValue("printer/baudrateIndex", ui->baudbox->currentIndex());
    settings.setValue("core/checktemperature", ui->checktemp->isChecked());
    if(firstrun) settings.setValue("core/firstrun", true);

    settings.beginWriteArray("user/recentfiles");
    for(int i = 0; i < recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("user/recentfile", recentFiles.at(i));
    }
    settings.endArray();

    delete ui;
}

void MainWindow::open()
{
    QString filename;
    QDir home;
    filename = QFileDialog::getOpenFileName(this, tr("Open GCODE"),home.home().absolutePath(), tr("GCODE (*.g *.gcode *.nc)"));

    gfile.setFileName(filename);
    if(!recentFiles.contains(filename))
    {
        if(recentFiles.size() < 5)
        {
            recentFiles.append(filename);
        }
        else
        {
           recentFiles.push_front(filename);
        }
    }

    parseFile(gfile);
}

void MainWindow::parseFile(QFile &file)
{
    gcode.clear();
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if(!line.startsWith(";"))
            {
                gcode.append(line);
            }

        }
        file.close();
        sdprinting = false;
        ui->fileBox->setEnabled(true);
        ui->filename->setText(file.fileName().split("/").last());
        ui->filelines->setText(QString::number(gcode.size()) + QString("/0 lines"));
    }
}

bool MainWindow::sendLine(QString line)
{

    if(printer.isOpen())
    {
        if(printer.write(line.toUtf8()+'\n'))
        {
            if(echo) printMsg(line + '\n');
            return true;
        }
        else return false;
    }
    else return false;

}

void MainWindow::serialupdate()
{
    ui->serialBox->clear();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(int i = 0; i < list.size(); i++)
    {
        ui->serialBox->addItem(list.at(i).portName());
    }
}

void MainWindow::serialconnect()
{
    if(!printer.isOpen())
    {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if(info.portName() == ui->serialBox->currentText())
            {
                printerinfo = info;
                break;
            }
        }

        printer.setPort(printerinfo);
        printer.setFlowControl(QSerialPort::NoFlowControl);

        switch(ui->baudbox->currentText().toInt())
        {
            case 4800:
            printer.setBaudRate(QSerialPort::Baud4800);
            break;

            case 9600:
            printer.setBaudRate(QSerialPort::Baud9600);
            break;

            case 115200:
            printer.setBaudRate(QSerialPort::Baud115200);
            break;

            default:
            printer.setBaudRate(ui->baudbox->currentText().toInt());
            break;
        }

        if(printer.open(QIODevice::ReadWrite))
        {
            ui->connectBtn->setText("Disconnect");
            ui->sendBtn->setDisabled(false);
            ui->pauseBtn->setDisabled(false);
            ui->progressBar->setValue(0);
            ui->controlBox->setDisabled(false);
            ui->consoleGroup->setDisabled(false);
            if(checkingTemperature) injectCommand("M105");
        }
    }

    else if(printer.isOpen())
    {
        printer.close();

        ui->connectBtn->setText("Connect");
        ui->sendBtn->setDisabled(true);
        ui->pauseBtn->setDisabled(true);
        ui->progressBar->setValue(0);
        ui->controlBox->setDisabled(true);
        ui->consoleGroup->setDisabled(true);

     }
}

//Buttons start
void MainWindow::xplus()
{
    QString command = "G91\nG1 X" + ui->stepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::xminus()
{
    QString command = "G91\nG1 X-" + ui->stepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::xhome()
{
    injectCommand("G28 X0");
}

void MainWindow::yplus()
{
    QString command = "G91\nG1 Y" + ui->stepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::yminus()
{
    QString command = "G91\nG1 Y-" + ui->stepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::yhome()
{
    injectCommand("G28 Y0");
}

void MainWindow::zplus()
{
    QString command = "G91\nG1 Z" + ui->stepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::zminus()
{
    QString command = "G91\nG1 Z-" + ui->stepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::zhome()
{
    injectCommand("G28 Z0");
}

void MainWindow::eplus()
{
    QString command = "G91\nG1 E" + ui->estepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::eminus()
{
    QString command = "G91\nG1 E-" + ui->estepspin->text() + "\nG90";
    injectCommand(command);
}

void MainWindow::ezero()
{
    injectCommand("G92 E0");
}

void MainWindow::homeall()
{
    injectCommand("G28");
}

void MainWindow::on_sendbtn_clicked()
{
    injectCommand(ui->sendtext->text());
}

void MainWindow::on_fanonbtn_clicked()
{
    injectCommand("M106");
}

void MainWindow::on_fanoffbtn_clicked()
{
    injectCommand("M107");
}

void MainWindow::on_atxonbtn_clicked()
{
    injectCommand("M80");
}

void MainWindow::on_atxoffbtn_clicked()
{
    injectCommand("M81");
}

void MainWindow::on_etmpset_clicked()
{
    QString command = "M80\nM104 S" + ui->etmpspin->text();
    injectCommand(command);
}

void MainWindow::on_etmpoff_clicked()
{
    injectCommand("M104 S0");
}

void MainWindow::on_btmpset_clicked()
{
    QString command = "M80\nM140 S" + ui->btmpspin->text();
    injectCommand(command);
}

void MainWindow::on_btmpoff_clicked()
{
    injectCommand("M140 S0");
}

void MainWindow::bedcenter()
{
    int x, y;

    if(settings.value("printer/bedx").toInt()) x = settings.value("printer/bedx").toInt();
    else x = 200;

    if(settings.value("printer/bedy").toInt()) y = settings.value("printer/bedy").toInt();
    else y = 200;

    QString command = "G1 X" + QString::number(x/2) + "Y" + QString::number(y/2);
    injectCommand(command);
}

//Buttons end

void MainWindow::readSerial()
{
    if(printer.canReadLine())
    {
        QByteArray data = printer.readLine();

        if(readingFiles)
        {
            if(!data.contains("End file list")) sdFiles.append(data);
            else
            {
                readingFiles = false;
                emit sdReady();
            }
        }

        if(data.startsWith("ok")) readyRecieve++;
        else if(data.startsWith("wait")) readyRecieve = 1;
        else if(checkingTemperature && data.startsWith("T:"))
        {
            QFuture<TemperatureReadings> parseThread = QtConcurrent::run(this, &MainWindow::parseStatus, data);
            statusWatcher.setFuture(parseThread); //parseThread is very costly operation
            ui->tempLine->setText(data);
        }
        else if(data.startsWith("Resend"))  //Handle resend if requested
        {
            if(currentLine > 0) currentLine -= data.split(':')[1].toInt();
            if(currentLine < 0) currentLine = 0;
        }
        else if(data.startsWith("Done")) sdprinting = false;
        else if(data.startsWith("SD printing byte") && sdWatcher.isFinished())
        {
            QFuture<double> parseSDThread = QtConcurrent::run(this, &MainWindow::parseSDStatus, data);
            sdWatcher.setFuture(parseSDThread);
        }
        else if(data.contains("Begin file list"))
        {
            sdFiles.clear();
            readingFiles = true; //start reading files from SD
        }

        printMsg(QString(data)); //echo
    }
}

void MainWindow::printMsg(const char* text)
{
    QTextCursor cursor = ui->terminal->textCursor();
    cursor.movePosition(QTextCursor::End);

    cursor.insertText(text);

    ui->terminal->setTextCursor(cursor);
;
}

void MainWindow::printMsg(QString text)
{
    QTextCursor cursor = ui->terminal->textCursor();
    cursor.movePosition(QTextCursor::End);

    cursor.insertText(text);

    ui->terminal->setTextCursor(cursor);
}

void MainWindow::on_sendBtn_clicked()
{
    if(sending && !sdprinting)
    {
        sending = false;
        ui->sendBtn->setText("Send");
        ui->pauseBtn->setText("Pause");
        ui->pauseBtn->setDisabled("true");
        ui->controlBox->setChecked("true");
        paused = false;
    }
    else if(!sending && !sdprinting)
    {
        sending=true;
        ui->sendBtn->setText("Stop");
        ui->pauseBtn->setText("Pause");
        ui->pauseBtn->setEnabled("true");
        ui->controlBox->setChecked("false");
        paused = false;
    }
    else if(sdprinting)
    {
        sending = false;
        sendLine("M24");
        ui->sendBtn->setText("Send");
        ui->pauseBtn->setText("Pause");
        ui->controlBox->setChecked("true");
        paused = false;
    }

    ui->progressBar->setValue(0);
    currentLine = 0;
}

void MainWindow::sendNext()
{
    if(injectingCommand && printer.isWritable() && readyRecieve > 0)
    {
        sendLine(userCommand);
        readyRecieve--;
        injectingCommand=false;
        return;
    }
    else if(sending && !paused && readyRecieve > 0 && !sdprinting && printer.isWritable())
    {
        if(currentLine >= gcode.size()) //check if we are at the end of array
        {
            sending = false;
            currentLine = 0;
            ui->sendBtn->setText("Send");
            ui->pauseBtn->setDisabled("true");
            ui->filelines->setText(QString::number(gcode.size()) + QString("/") + QString::number(currentLine) + QString(" Lines"));
            return;
        }
        sendLine(gcode.at(currentLine));
        currentLine++;
        readyRecieve--;

        ui->filelines->setText(QString::number(gcode.size()) + QString("/") + QString::number(currentLine) + QString(" Lines"));
        ui->progressBar->setValue(((float)currentLine/gcode.size()) * 100);
    }
}

void MainWindow::on_pauseBtn_clicked()
{
    if(paused && !sdprinting)
    {
        paused = false;
        ui->controlBox->setChecked(false);
        ui->pauseBtn->setText("Pause");
    }
    else if(!paused && !sdprinting)
    {
        paused = true;
        ui->controlBox->setChecked(true);
        ui->pauseBtn->setText("Resume");
    }
    else if(sdprinting)
    {
        injectCommand("M25");
    }
}

void MainWindow::checkStatus()
{
    if(checkingTemperature && (sinceLastTemp.elapsed() > statusTimer.interval())) injectCommand("M105");
}

void MainWindow::on_checktemp_stateChanged(int arg1)
{
    if(arg1) checkingTemperature = true;
    else checkingTemperature = false;
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsWindow settingswindow(this);

    settingswindow.exec();
}

void MainWindow::on_releasebtn_clicked()
{
    injectCommand("M84");
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWindow aboutwindow(this);

    aboutwindow.exec();
}

void MainWindow::injectCommand(QString command)
{
    injectingCommand = true;
    userCommand = command;
}

void MainWindow::updateRecent()
{
    //TODO
}

void MainWindow::serialError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::NoError) return;
    if(error == QSerialPort::NotOpenError) return;

    if(printer.isOpen()) printer.close();

    if(sending) paused = true;

    ui->connectBtn->setText("Connect");
    ui->sendBtn->setDisabled(true);
    ui->pauseBtn->setDisabled(true);
    ui->controlBox->setDisabled(true);
    ui->consoleGroup->setDisabled(true);

    qDebug() << error;

    QString errorMsg;
    switch(error)
    {
    case QSerialPort::DeviceNotFoundError:
        errorMsg = "Device not found";
        break;

    case QSerialPort::PermissionError:
        errorMsg = "Insufficient permissions\nAlready opened?";
        break;

    case QSerialPort::OpenError:
        errorMsg = "Cant open port\nAlready opened?";
        break;

    case QSerialPort::TimeoutError:
        errorMsg = "Serial connection timed out";
        break;

    case QSerialPort::WriteError:
    case QSerialPort::ReadError:
        errorMsg = "I/O Error";
        break;

    case QSerialPort::ResourceError:
        errorMsg = "Disconnected";
        break;

    default:
        errorMsg = "Unknown error\nSomething went wrong";
        break;
    }

    ErrorWindow errorwindow(this, errorMsg);
    errorwindow.exec();
}

TemperatureReadings MainWindow::parseStatus(QByteArray data)
{
    QString extmp = "";
    QString btmp = "";

    for(int i = 2; data.at(i) != '/'; i++)
    {
        extmp+=data.at(i);
    }
    for(int i = data.indexOf("B:")+2; data.at(i) != '/'; i++)
    {
        btmp+=data.at(i);
    }

    //ui->extruderlcd->display(extmp.toDouble()); //Not needed here, moved to updateStatus();
    //ui->bedlcd->display(btmp.toDouble());
    //sinceLastTemp.restart();

    TemperatureReadings t;
    t.e = extmp.toDouble();
    t.b = btmp.toDouble();

    return t;
}

void MainWindow::updateStatus()
{
    TemperatureReadings r = statusWatcher.future().result();
    ui->extruderlcd->display(r.e);
    ui->bedlcd->display(r.b);

    sinceLastTemp.restart();
}

void MainWindow::on_actionPrint_from_SD_triggered()
{
    injectCommand("M20");
}

void MainWindow::initSDprinting()
{
    SDWindow sdwindow(sdFiles, this);

    connect(&sdwindow, SIGNAL(fileSelected(QString)), this, SLOT(selectSDfile(QString)));

    sdwindow.exec();
}

double MainWindow::parseSDStatus(QByteArray data)
{
    QString tmp;
    QString fragment = data.split(' ').at(3);
    for(int i = 0; fragment.at(i) != '/'; ++i)
    {
        tmp += fragment.at(i);
    }

    return tmp.toDouble();
}

void MainWindow::selectSDfile(QString file)
{
    ui->filename->setText(file.split(" ")[0]);
    ui->filelines->setText(file.split(" ")[1] + QString("/0 bytes"));
    ui->progressBar->setValue(0);
    sdBytes = file.split(" ")[1].toDouble();

    sendLine("M23 " + file.split(" ")[0]);
    sdprinting = true;
    ui->fileBox->setDisabled(false);
}

void MainWindow::updateSDStatus()
{
    double currentSDbytes = sdWatcher.future().result();
    ui->filelines->setText(QString::number(sdBytes) + QString("/") + QString::number(currentSDbytes) + QString(" bytes"));
    ui->progressBar->setValue(currentSDbytes/sdBytes * 100);
    if(currentSDbytes == sdBytes) sdprinting = false;
}

void MainWindow::checkSDStatus()
{
    if(sdWatcher.isFinished() && sdprinting) injectCommand("M27");
}

void MainWindow::on_stepspin_valueChanged(const QString &arg1)
{
    if(arg1.toFloat() < 1) ui->stepspin->setSingleStep(0.1);
    else if(arg1.toFloat() >=10) ui->stepspin->setSingleStep(10);
    else if(arg1.toFloat() >= 1) ui->stepspin->setSingleStep(1);
}

void MainWindow::on_estepspin_valueChanged(const QString &arg1)
{
    if(arg1.toFloat() < 1) ui->estepspin->setSingleStep(0.1);
    else if(arg1.toFloat() >=10) ui->estepspin->setSingleStep(10);
    else if(arg1.toFloat() >= 1) ui->estepspin->setSingleStep(1);
}
