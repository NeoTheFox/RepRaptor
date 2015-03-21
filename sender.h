#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QQueue>
#include <QTimer>
#include <QSettings>
#include <QVector>

#include "repraptor.h"

using namespace RepRaptor;

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = 0);
    ~Sender();

protected:
    QSerialPort *printer;
    QTimer *sendTimer;
    long int currentLine;
    long int totalLineNum;
    long int resendNum;
    long int baudrate;
    long int flowcontrol;
    bool paused;
    bool sending;
    bool dtr;
    bool readyReceive;
    bool sendingChecksum;
    bool resending;
    QQueue <QString> userCommands;
    QStringList sentCommands;
    QVector <QString> gcode;

    bool sendLine(QString s);

signals:
    void errorReceived(QSerialPort::SerialPortError error);
    void dataReceived(QByteArray data);
    void reportProgress(FileProgress p);
    void baudrateSetFailed(int b);

public slots:
    void openPort(QSerialPortInfo i);
    void closePort();
    void startPrinting();
    void stopPrinting();
    void pause(bool p);
    void setBaudrate(int b);
    void setFile(QVector <QString> f);
    void injectCommand(QString);
    void flushInjectionBuffer();

    void receivedOkWait();
    void receivedOkNum(int);
    void receivedStart();
    void receivedResend(int r);

    void sendNext();

    void receivedData();
    void receivedError(QSerialPort::SerialPortError error);
};

#endif // SENDER_H
