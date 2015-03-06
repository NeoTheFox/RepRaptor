#ifndef REPRAPTOR_H
#define REPRAPTOR_H

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
}

#endif // REPRAPTOR_H

