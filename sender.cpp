#include "sender.h"

Sender::Sender(QObject *parent) : QObject(parent)
{
    //Initial values
    currentLine=0;
    totalLineNum=0;
    baudrate=115200;
    resendNum = 0;
    resending = false;
    sendingChecksum=false;
    paused=false;
    sending=false;
    readyReceive = false;
    printer = new QSerialPort(this);
    sendTimer = new QTimer(this);

    //Fetch settings
    QSettings settings(this);
    sendTimer->setInterval(settings.value("core/senderinterval", 2).toInt());
    sendingChecksum = settings.value("core/checksums", 0).toBool();
    dtr = settings.value("core/dtr", 1).toBool();
    flowcontrol = settings.value("core/flowcontrol", 0).toInt();

    sendTimer->start();

    connect(printer, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(receivedError(QSerialPort::SerialPortError)));
    connect(printer, &QSerialPort::readyRead, this, &Sender::receivedData);
    connect(sendTimer, &QTimer::timeout, this, &Sender::sendNext);
}

Sender::~Sender()
{
    closePort();
    sendTimer->stop();
}

//Mainloop of sending
void Sender::sendNext()
{
    if(printer->isWritable() && readyReceive)
    {
        //Checksums
        if(sendingChecksum && resending)
        {
            if(resendNum < sentCommands.size())
            {
                sendLine(sentCommands.at(resendNum));
                resendNum++;
            }
            else if(resendNum == sentCommands.size())
            {
                resending = false;
                resendNum = 0;
            }
            else if(resendNum > sentCommands.size())
            {
                sendLine("M110 N0");
                totalLineNum = 0;
                resendNum = 0;
                sentCommands.clear();
            }
            return;
        }
        if(!userCommands.isEmpty()) //Inject user command
        {
            sendLine(userCommands.dequeue());
            readyReceive = false;
            return;
        }
        else if(sending && !paused) //Send line of gcode
        {
            FileProgress p;
            if(currentLine >= gcode.size()) //check if we are at the end of array
            {
                sending = false;
                currentLine = 0;
                if(sendingChecksum) sendLine("M110 N0");

                p.P = gcode.size();
                p.T = gcode.size();
                emit reportProgress(p);

                return;
            }
            sendLine(gcode.at(currentLine));
            currentLine++;
            readyReceive=false;

            p.P = currentLine;
            p.T = gcode.size();
            emit reportProgress(p);
        }
    }
}

bool Sender::sendLine(QString line)
{
    sentCommands.clear();
    if(printer->isOpen())
    {
        if(sendingChecksum)
        {
            if(line.contains("M110")) totalLineNum = 0;
            sentCommands.append(line);
            //Checksum algorithm from RepRap wiki
            line = "N"+QString::number(totalLineNum)+line+"*";
            int cs = 0;
            for(int i = 0; line.at(i) != '*'; i++) cs = cs ^ line.at(i).toLatin1();
            cs &= 0xff;
            line += QString::number(cs);
            totalLineNum++;
        }
        if(printer->write(line.toUtf8()+'\n')) return true;
        else return false;
    }
    else return false;
}

void Sender::openPort(QSerialPortInfo i)
{
    printer->setPort(i);

    if(!printer->isOpen() && printer->open(QIODevice::ReadWrite))
    {
        //Moved here to be compatible with Qt 5.2.1
        printer->setDataTerminalReady(dtr);
        if(!printer->setBaudRate(baudrate))
            emit baudrateSetFailed(baudrate);
        printer->setFlowControl(static_cast<QSerialPort::FlowControl>(flowcontrol));
    }

    readyReceive = true;
}

void Sender::closePort()
{
    if(printer->isOpen()) printer->close();
}

void Sender::startPrinting()
{
    currentLine = 0;
    paused = false;
    sending = true;
}

void Sender::stopPrinting()
{
    currentLine = 0;
    paused = false;
    sending = false;
}

void Sender::pause(bool p)
{
    paused = p;
}

void Sender::setBaudrate(int b)
{
    baudrate = b;
}

void Sender::setFile(QVector <QString> f)
{
    gcode = f;
}

void Sender::injectCommand(QString command)
{
    if(!userCommands.contains(command)) userCommands.enqueue(command);
}

void Sender::receivedOkWait()
{
    readyReceive = true;
}

void Sender::receivedOkNum(int)
{
    readyReceive = true;
}

void Sender::receivedStart()
{
    readyReceive = true;
}

void Sender::flushInjectionBuffer()
{
    userCommands.clear();
}

void Sender::receivedResend(int r)
{
    if(sendingChecksum)
    {
        resending = true;
        resendNum = r;
    }
    else currentLine--;
}

void Sender::receivedData()
{
    if(printer->canReadLine())
    {
        QByteArray data = printer->readLine();
        if(data == "") return;
        emit dataReceived(data);
        //Yeah, yeah, I know. This class is called "Sender", but checking this here is faster.
        if(data.startsWith("ok") || data.startsWith("wait")) readyReceive=true;
    }
}

void Sender::receivedError(QSerialPort::SerialPortError error)
{
    if(error > 0)
    {
        closePort();
        emit errorReceived(error);
    }
}
