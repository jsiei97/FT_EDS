
#include "EEPROM.h"
#include "FT_EDS.h"

#include <QByteArray>
#include <QDebug>

EEPROMClass::EEPROMClass()
{
    prom.fill(0xFF, EEPROM_MAX_SIZE);
}

uint8_t EEPROMClass::read(int address)
{
    if(address>=EEPROM_MAX_SIZE)
    {
        qDebug() << __func__ << __LINE__ << 
            "Warning logic error, address to big!" << 
            address << EEPROM_MAX_SIZE;
        return 0;
    }

    if(prom.isNull())
    {
        //qDebug() << __func__ << __LINE__ << "New array";
        prom.fill(0xFF, EEPROM_MAX_SIZE);
        //return 0;
    }

    return prom[address];
}

void EEPROMClass::write(int address, uint8_t value)
{
    if(address>=EEPROM_MAX_SIZE)
    {
        qDebug() << __func__ << __LINE__ << 
            "Warning logic error, address to big!" << 
            address << EEPROM_MAX_SIZE;
        return;
    }

    if(prom.isNull())
    {
        //qDebug() << __func__ << __LINE__ << "New array";
        //prom = new QByteArray(1024);
    }
    //eeprom_write_byte((unsigned char *) address, value);

    prom[address] = value;
    //qDebug() << prom.toHex();
}

EEPROMClass EEPROM;
