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
    QRegExp temperatureRegxp;
    int firmware;
    bool readingFiles;
    bool readingEEPROM;
    bool EEPROMReadingStarted;

signals:
    void receivedTemperature(TemperatureReadings);
    void receivedSDUpdate(SDProgress);
    void receivedNotSDPrinting();
    void receivedEEPROMLine(QString);
    void recievingEEPROMDone();
    void receivedSDFilesList(QStringList);
    void receivedOkWait();
    void receivedOkNum(int);
    void receivedStart();
    void receivedResend(int);
    void receivedError();
    void receivedFirmware(int);
    void receivedSDDone();

public slots:
    void parse(QByteArray data);
    void setEEPROMReadingMode();
};

#endif // PARSETHREAD_H
