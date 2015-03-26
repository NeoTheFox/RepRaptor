#include "parser.h"

Parser::Parser(QObject *parent):
    QObject(parent)
{
    //Init regexp
    temperatureRegxp.setCaseSensitivity(Qt::CaseInsensitive);
    temperatureRegxp.setPatternSyntax(QRegExp::RegExp);
    temperatureRegxp.setPattern("\\d+\\.\\d+"); // Find float in string

    //Init values
    readingFiles = false;
    readingEEPROM = false;
    EEPROMReadingStarted = false;

    //Load settings
    QSettings settings;
    firmware = settings.value("printer/firmware").toInt();
}

Parser::~Parser()
{

}

void Parser::parse(QByteArray data)
{
    if(!data.isEmpty()) //Dont need to process empty strings
    {
        if(readingFiles) //SD files list reading mode
        {
            if(!data.contains("End file list")) SDFilesList.append(data.remove(data.size()-2, 2));
            else
            {
                readingFiles = false;
                emit receivedSDFilesList(SDFilesList);
            }
            return;
        }

        if(readingEEPROM) // EEPROM reading mode
        {
            if(firmware == Repetier)
            {
                if(data.startsWith("EPR"))
                {
                    emit receivedEEPROMLine(QString(data));
                    EEPROMReadingStarted = true;
                }
                else if(EEPROMReadingStarted)
                {
                    readingEEPROM = false;
                    EEPROMReadingStarted = false;
                    emit recievingEEPROMDone();
                }

                return;
            }
        }
        /*
        if(data.startsWith("ok"))
        {
            QStringList tmp = QString(data).split(' ');
            if(tmp.size() > 1)
                emit receivedOkNum(tmp.at(1).toInt());
            else emit receivedOkNum(0);
        }
        */
        else if(data.contains("T:"))
        {
            TemperatureReadings r;

            if(temperatureRegxp.indexIn(QString(data)) != -1)
                r.e = temperatureRegxp.cap(0).toDouble();
            else r.e = 0;
            if(temperatureRegxp.indexIn(QString(data), temperatureRegxp.matchedLength()) != -1)
                r.b = temperatureRegxp.cap(0).toDouble();
            else r.b = 0;

            r.raw = QString(data);

            emit receivedTemperature(r);
        }
        //else if(data.startsWith("wait")) emit receivedOkWait();
        else if(data.startsWith("rs") || data.toLower().startsWith("resend"))
            emit receivedResend(data.split(':').at(1).toInt());
        else if(data.startsWith("!!")) emit receivedError();
        else if(data.startsWith("Done")) emit receivedSDDone();
        else if(data.startsWith("start")) emit receivedStart();
        else if(data.startsWith("SD pr"))
        {
            SDProgress p;
            QRegExp rxp("\\d+/\\d+");
            QStringList tmp;

            if(rxp.indexIn(data) != -1)
            {
                tmp = rxp.cap(0).split('/');

                p.progress = tmp.at(0).toLong();
                p.total = tmp.at(1).toLong();
            }
            else return;

            emit receivedSDUpdate(p);

        }
        else if(data.startsWith("Not SD ")) emit receivedNotSDPrinting();
        else if(data.contains("Begin file list"))
        {
            SDFilesList.clear();
            readingFiles = true; //start reading files from SD
        }
        //else if(data.contains("REPETIER")) emit receivedFirmware(Repetier);
        //else if(data.contains("MARLIN")) emit receivedFirmware(Marlin);
    }
}


void Parser::setEEPROMReadingMode()
{
    readingEEPROM = true;
}
