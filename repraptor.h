#ifndef REPRAPTOR_H
#define REPRAPTOR_H

#include <QString>

namespace RepRaptor
{
    typedef struct
    {
        double e, b;
        QString raw;
    } TemperatureReadings;

    enum Firmware
    {
        Marlin,
        Repetier,
        OtherFirmware
    };

    typedef struct
    {
        int T, P;
        QString S;
    } repetierEEPROMline;

    typedef struct
    {
        unsigned long int progress, total;
    } SDProgress;

    typedef struct
    {
        unsigned int T, P;
    } FileProgress;
}

#endif // REPRAPTOR_H

