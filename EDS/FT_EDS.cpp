/**
 * @file FT_EDS.cpp
 * @author Johan Simonsson
 * @brief The Fun Tech EEPROM Data Storage class
 */

#include "FT_EDS.h"
#include <EEPROM.h>

//#include <QDebug>
// --- PRIVATE ---

uint16_t FT_EDS::read16(int address)
{
    uint16_t data = EEPROM.read(address);
    data <<= 8;
    data += EEPROM.read(address+1);
    return data;
}


uint32_t FT_EDS::read32(int address)
{
    uint32_t data = 0;
    for( int i=0 ; i<4 ; i++ )
    {
        data <<= 8;
        data += EEPROM.read(address+i);
    }
    return data;
}

void FT_EDS::write16(int address, uint16_t data)
{
    //qDebug() << __func__ << __LINE__ << address << data;
    for(int i=1; i>=0; i--)
    {
        EEPROM.write(address+i, (0xFF&data));
        data >>= 8;
    }
}

void FT_EDS::write32(int address, uint32_t data)
{
    //qDebug() << __func__ << __LINE__ << address << data;
    for(int i=3; i>=0; i--)
    {
        EEPROM.write(address+i, (0xFF&data));
        data >>= 8;
    }
}
// --- PUBLIC ---

void FT_EDS::init()
{
    //check for the magic and a valid base
    if((EEPROM.read(0) == MAGIC_0) &&
            (EEPROM.read(1) == MAGIC_1) &&
            (EEPROM.read(2) == MAGIC_2) &&
            (EEPROM.read(3) == MAGIC_3) &&
            (EEPROM.read(4) == FT_EDS_REV))
    {
        //ok!
        return;
    }
    else
    {
        //No valid data...!
        //let's create a new base
        EEPROM.write(0, MAGIC_0);
        EEPROM.write(1, MAGIC_1);
        EEPROM.write(2, MAGIC_2);
        EEPROM.write(3, MAGIC_3);
        EEPROM.write(4, FT_EDS_REV);

        //with DEC (Data Entry Count) 0
        EEPROM.write(5, 0);
        EEPROM.write(6, 0);
    }

    posNextDE = 7;
    posFreeData = EEPROM_MAX_SIZE-1;
}

uint16_t FT_EDS::getDEC()
{
    return read16(5);
}

int FT_EDS::getDE(edsId id, uint8_t* data)
{
    return 0;
}

bool FT_EDS::updateDE(edsId id, edsType type, uint8_t* data, unsigned int len)
{
    unsigned int pos = 0;
    uint16_t dec = getDEC();
    for( int i = 0 ; i < dec ; i++ )
    {
        pos = 7+(i*12);
        if(read16(pos)==(uint16_t)id)
        {
            break;
        }
        else
        {
            pos = 0;
        }
    }

    if(0 == pos)
    {
        //add a new DE, since this is new
        dec++;
        write16(5, dec);

         //posNextDE = 7;
         //posFreeData = EEPROM_MAX_SIZE-1;

        write16(posNextDE,   (uint16_t)id);   //deId
        write16(posNextDE+2, (uint16_t)type); //deType
        write32(posNextDE+4, (uint32_t)len);  //deLen

        if(len < 4)
        {
            //data fits in the area
        }
        else
        {
            //data goes into the free area, deData has a pointer to it!
            posFreeData -= len;
            write32(posNextDE+8, (uint32_t)(posFreeData));  //deLen

            for( int i=0 ; i<len ; i++ )
            {
                EEPROM.write(posFreeData+i, data[i]);
            }
            
            posFreeData--;
        }

    }
    else
    {
        //update the old DE found at "pos"
    }

}


