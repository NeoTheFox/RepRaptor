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
    unsigned int currentLine;
    unsigned int totalLineNum;
    unsigned int resendNum;
    unsigned int baudrate;
    bool paused;
    bool sending;
    bool dtr;
    bool readyRecieve;
    bool sendingChecksum;
    bool resending;
    QQueue <QString> userCommands;
    QStringList sentCommands;
    QVector <QString> gcode;

    bool sendLine(QString s);

signals:
    void errorRecieved(QSerialPort::SerialPortError error);
    void dataRecieved(QByteArray data);
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

    void recievedOkWait();
    void recievedOkNum(int);
    void recievedStart();
    void recievedResend(int r);

    void sendNext();

    void recievedData();
    void recievedError(QSerialPort::SerialPortError error);
};

#endif // SENDER_H
