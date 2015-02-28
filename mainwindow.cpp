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
    ui->baudbox->setCurrentIndex(2);

    ui->extruderlcd->setPalette(Qt::red);
    ui->bedlcd->setPalette(Qt::red);

    sending = false;
    paused = false;
    commandDone = false;
    checkingTemperature = true;
    currentLine = 0;

    serialupdate();

    connect(&printer, SIGNAL(readyRead()), this, SLOT(readSerial()));
    connect(&statusTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
    connect(&sendTimer, SIGNAL(timeout()), this, SLOT(sendNext()));

    if(settings.value("core/statusinterval").toInt()) statusTimer.setInterval(settings.value("core/statusinterval").toInt());
    else statusTimer.setInterval(3000);
    statusTimer.start();

    if(settings.value("core/senderinterval").toInt()) sendTimer.setInterval(settings.value("core/senderinterval").toInt());
    else sendTimer.setInterval(5);
    sendTimer.start();
}

MainWindow::~MainWindow()
{
    if(gfile.isOpen()) gfile.close();
    if(printer.isOpen()) printer.close();

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
            printMsg(line);
            return true;
        }
        else
        {
            return false;
        }
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
        printer.setFlowControl(QSerialPort::HardwareControl);

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
            printer.setBaudRate(QSerialPort::Baud115200);
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
    QString command = "G91\n G1 X" + ui->stepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::xminus()
{
    QString command = "G91\n G1 X-" + ui->stepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::xhome()
{
    sendLine("G28 X0");
}

void MainWindow::yplus()
{
    QString command = "G91\n G1 Y" + ui->stepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::yminus()
{
    QString command = "G91\n G1 Y-" + ui->stepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::yhome()
{
    sendLine("G28 Y0");
}

void MainWindow::zplus()
{
    QString command = "G91\n G1 Z" + ui->stepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::zminus()
{
    QString command = "G91\n G1 Z-" + ui->stepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::zhome()
{
    sendLine("G28 Z0");
}

void MainWindow::eplus()
{
    QString command = "G91\n G1 E" + ui->estepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::eminus()
{
    QString command = "G91\n G1 E-" + ui->estepspin->text() + "\n G90";
    sendLine(command);
}

void MainWindow::ezero()
{
    sendLine("G92 E0");
}

void MainWindow::homeall()
{
    sendLine("G28");
}

void MainWindow::on_sendbtn_clicked()
{
    sendLine(ui->sendtext->text());
}

void MainWindow::on_fanonbtn_clicked()
{
    sendLine("M106");
}

void MainWindow::on_fanoffbtn_clicked()
{
    sendLine("M107");
}

void MainWindow::on_atxonbtn_clicked()
{
    sendLine("M80");
}

void MainWindow::on_atxoffbtn_clicked()
{
    sendLine("M81");
}

void MainWindow::on_etmpset_clicked()
{
    QString command = "M104 S" + ui->etmpspin->text();
    sendLine(command);
}

void MainWindow::on_etmpoff_clicked()
{
    sendLine("M104 S0");
}

void MainWindow::on_btmpset_clicked()
{
    QString command = "M140 S" + ui->btmpspin->text();
    sendLine(command);
}

void MainWindow::on_btmpoff_clicked()
{
    sendLine("M140 S0");
}

void MainWindow::bedcenter()
{
    int x, y;

    if(settings.value("printer/bedx").toInt()) x = settings.value("printer/bedx").toInt();
    else x = 200;

    if(settings.value("printer/bedy").toInt()) y = settings.value("printer/bedy").toInt();
    else y = 200;

    QString command = "G1 X" + QString::number(x/2) + "Y" + QString::number(y/2);
    sendLine(command);
}

//Buttons end

void MainWindow::readSerial()
{
    if(printer.canReadLine())
    {
        QByteArray data = printer.readLine();
        if(data.startsWith("T:"))
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

           ui->extruderlcd->display(extmp.toDouble());
           ui->bedlcd->display(btmp.toDouble());
           sinceLastTemp.restart();
        }
        else if(data.startsWith("ok") || data.startsWith("wait")) commandDone = true;
        printMsg(QString(data));
    }
}

void MainWindow::printMsg(const char* text)
{
    QTextCursor cursor = ui->terminal->textCursor();
    cursor.movePosition(QTextCursor::End);

    //QTextBlockFormat bf;
    //QTextCharFormat cf;

    //cursor.insertBlock(bf, cf);
    cursor.insertText(text);
    cursor.insertText("\n");

    ui->terminal->setTextCursor(cursor);
;
}

void MainWindow::printMsg(QString text)
{
    QTextCursor cursor = ui->terminal->textCursor();
    cursor.movePosition(QTextCursor::End);

    //QTextBlockFormat bf;
    //QTextCharFormat cf;

    //cursor.insertBlock(bf, cf);
    cursor.insertText(text);
    cursor.insertText("\n");

    ui->terminal->setTextCursor(cursor);
}

void MainWindow::on_sendBtn_clicked()
{
    if(sending)
    {
        sending = false;
        ui->sendBtn->setText("Send");
        ui->pauseBtn->setDisabled("true");
    }
    else if(!sending)
    {
        sending=true;
        ui->sendBtn->setText("Stop");
        ui->pauseBtn->setEnabled("true");
    }

    ui->progressBar->setValue(0);
    currentLine = 0;
}

void MainWindow::sendNext()
{
    if(sending && !paused && printer.bytesToWrite() < 100 && commandDone)
    {
        if(currentLine >= gcode.size()) //check if we are at the end of array
        {
            sending = false;
            currentLine = 0;
            ui->sendBtn->setText("Send");
            ui->pauseBtn->setDisabled("true");
            return;
        }
        sendLine(gcode.at(currentLine));
        currentLine++;
        commandDone = false;

        ui->filelines->setText(QString::number(gcode.size()) + QString("/") + QString::number(currentLine) + QString(" Lines"));
        ui->progressBar->setValue(((float)currentLine/gcode.size()) * 100);
    }
}

void MainWindow::on_pauseBtn_clicked()
{
    if(paused)
    {
        paused = false;
        ui->pauseBtn->setText("Pause");
    }
    else
    {
        paused = true;
        ui->pauseBtn->setText("Resume");
    }
}

void MainWindow::checkStatus()
{
    if(checkingTemperature && sinceLastTemp.elapsed() < statusTimer.interval()) sendLine("M105");
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
    sendLine("M84");
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWindow aboutwindow(this);

    aboutwindow.exec();
}

void MainWindow::updateRecent()
{

}
