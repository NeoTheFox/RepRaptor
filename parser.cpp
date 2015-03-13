#include "parser.h"

Parser::Parser(QObject *parent):
    QObject(parent)
{
    this->setParent(parent);
    temperatureRegxp.setCaseSensitivity(Qt::CaseInsensitive);
    temperatureRegxp.setPatternSyntax(QRegExp::RegExp);
    temperatureRegxp.setPattern("\\d+\\.\\d+"); // Find float in string

    readingFiles = false;
    readingEEPROM = false;
    EEPROMReadingStarted = false;

    QSettings settings;
    firmware = settings.value("printer/firmware").toInt();
}

Parser::~Parser()
{

}

void Parser::parse(QByteArray data)
{
    if(!data.isEmpty())
    {
        if(readingFiles)
        {
            if(!data.contains("End file list")) SDFilesList.append(data);
            else
            {
                readingFiles = false;
                emit recievedSDFilesList(SDFilesList);
            }
            return;
        }

        if(readingEEPROM)
        {
            if(firmware == Repetier)
            {
                if(data.startsWith("EPR"))
                {
                    emit recievedEEPROMLine(QString(data));
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
                emit recievedOkNum(tmp.at(1).toInt());
            else emit recievedOkNum(0);
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

            emit recievedTemperature(r);
        }
        //else if(data.startsWith("wait")) emit recievedOkWait();
        else if(data.startsWith("rs") || data.toLower().startsWith("resend"))
            emit recievedResend(data.split(' ').at(1).toInt());
        else if(data.startsWith("!!")) emit recievedError();
        else if(data.startsWith("Done")) emit recievedSDDone();
        else if(data.startsWith("start")) emit recievedStart();
        else if(data.startsWith("SD pr"))
        {
            /*
            QString tmp;
            QString fragment = data.split(' ').at(3);
            for(int i = 0; fragment.at(i) != '/'; ++i)
            {
                tmp += fragment.at(i);
            }
            emit recievedSDUpdate(tmp.toDouble());
            */
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

            emit recievedSDUpdate(p);

        }
        else if(data.startsWith("Not SD "));
        else if(data.contains("Begin file list"))
        {
            SDFilesList.clear();
            readingFiles = true; //start reading files from SD
        }
        //else if(data.contains("REPETIER")) emit recievedFirmware(Repetier);
        //else if(data.contains("MARLIN")) emit recievedFirmware(Marlin);
    }
}


void Parser::setEEPROMReadingMode()
{
    readingEEPROM = true;
}
