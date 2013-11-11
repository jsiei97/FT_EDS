/**
 * @file EEPROM.h
 * @author Johan Simonsson  
 * @brief A stub version of the Arduino EEPROM class
 */


#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>
#include <QByteArray>

class EEPROMClass
{
    public:
        QByteArray prom;
        uint8_t read(int);
        void write(int, uint8_t);
};

extern EEPROMClass EEPROM;

#endif

