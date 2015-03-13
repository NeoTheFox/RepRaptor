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

    QSerialPort *printer;

protected:
    int currentLine;
    int totalLineNum;
    int baudrate;
    bool paused;
    bool sending;
    bool readyRecieve;
    bool sendingChecksum;
    QTimer sendTimer;
    QQueue <QString> userCommands;
    QStringList sentCommands;
    QVector <QString> gcode;

    bool sendLine(QString s);

signals:
    void errorRecieved(QSerialPort::SerialPortError error);
    void dataRecieved(QByteArray data);
    void reportProgress(FileProgress p);

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
    void recievedResend(int);

    void sendNext();

    void recievedData();
    void recievedError(QSerialPort::SerialPortError error);
};

#endif // SENDER_H
