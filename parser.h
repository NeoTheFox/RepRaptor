#ifndef PARSETHREAD_H
#define PARSETHREAD_H

#include <QThread>
#include <QSettings>

#include "repraptor.h"

using namespace RepRaptor;

class Parser : public QObject
{
    Q_OBJECT

public:
    explicit Parser(QObject *parent = 0);
    ~Parser();

protected:
    QByteArray data;
    QStringList SDFilesList;
    int firmware;
    bool readingFiles;
    bool readingEEPROM;
    bool EEPROMReadingStarted;
    QRegExp temperatureRegxp;

signals:
    void recievedTemperature(TemperatureReadings);
    void recievedSDUpdate(double);
    void recievedEEPROMLine(QString);
    void recievingEEPROMDone();
    void recievedSDFilesList(QStringList);
    void recievedOkWait();
    void recievedOkNum(int);
    void recievedStart();
    void recievedResend(int);
    void recievedError();
    void recievedFirmware(int);
    void recievedSDDone();

public slots:
    void parse(QByteArray data);
    void setEEPROMReadingMode();
};

#endif // PARSETHREAD_H
