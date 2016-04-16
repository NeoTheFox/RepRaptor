/////////////////////////////////////////////////
//This file contains RepRaptor - specific stuff//
/////////////////////////////////////////////////

#ifndef REPRAPTOR_H
#define REPRAPTOR_H

#ifndef REPRAPTOR_VERSION
#define REPRAPTOR_VERSION "0.3.9"
#endif

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

    enum ErrorType
    {
        SerialPortError,
        OpenFileError,
        HardwareFailure
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

