#include "sender.h"

Sender::Sender(QObject *parent) : QObject(parent)
{
    //Initial values
    currentLine=0;
    totalLineNum=0;
    baudrate=115200;
    sendingChecksum=false;
    paused=false;
    sending=false;
    readyRecieve = false;
    printer = new QSerialPort(this);

    //Fetch settings
    QSettings settings;
    sendTimer.setInterval(settings.value("core/senderinterval", 2).toInt());
    sendingChecksum = settings.value("core/checksums", 0).toBool();

    sendTimer.start();

    connect(printer, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(recievedError(QSerialPort::SerialPortError)));
    connect(printer, &QSerialPort::readyRead, this, &Sender::recievedData);
    connect(&sendTimer, &QTimer::timeout, this, &Sender::sendNext);
}

Sender::~Sender()
{
    closePort();
    sendTimer.stop();
}

//Mainloop of sending
void Sender::sendNext()
{
    if(printer->isWritable())
    {
        if(!userCommands.isEmpty() && readyRecieve) //Inject user command
        {
            sendLine(userCommands.dequeue());
            readyRecieve = false;
            return;
        }
        else if(sending && !paused && readyRecieve) //Send line of gcode
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
            readyRecieve=false;

            p.P = currentLine;
            p.T = gcode.size();
            emit reportProgress(p);
        }
    }
}

bool Sender::sendLine(QString line)
{
    if(printer->isOpen())
    {
        if(sendingChecksum)
        {
            if(line.contains("M110")) totalLineNum = 0;

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
        switch(baudrate)
        {
            case 4800:
            printer->setBaudRate(QSerialPort::Baud4800);
            break;

            case 9600:
            printer->setBaudRate(QSerialPort::Baud9600);
            break;

            case 115200:
            printer->setBaudRate(QSerialPort::Baud115200);
            break;

            default:
            printer->setBaudRate(baudrate);
            break;
        }

        printer->setFlowControl(QSerialPort::HardwareControl);
    }
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
    userCommands.enqueue(command);
}

void Sender::recievedOkWait()
{
    readyRecieve = true;
}

void Sender::recievedOkNum(int)
{
    readyRecieve = true;
}

void Sender::recievedStart()
{
    readyRecieve = true;
}

void Sender::flushInjectionBuffer()
{
    userCommands.clear();
}

void Sender::recievedResend(int)
{

}

void Sender::recievedData()
{
    if(printer->canReadLine())
    {
        QByteArray data = printer->readLine();
        emit dataRecieved(data);
        if(data.startsWith("ok") || data.startsWith("wa")) readyRecieve=true;
    }
}

void Sender::recievedError(QSerialPort::SerialPortError error)
{
    if(error > 0)
    {
        closePort();
        emit errorRecieved(error);
    }
}
