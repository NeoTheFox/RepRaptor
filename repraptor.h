#ifndef REPRAPTOR_H
#define REPRAPTOR_H

#include <QString>

namespace RepRaptor
{
    typedef struct
    {
        double e, b;
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
}

#endif // REPRAPTOR_H

