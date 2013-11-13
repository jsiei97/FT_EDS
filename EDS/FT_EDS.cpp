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
        uint16_t dec = getDEC();
        posNextDE = 7+(dec*10);
        posFreeData = EEPROM_MAX_SIZE;

        //check the DE with len > 4 for the lowest address
        for( unsigned int i = 0 ; i < dec ; i++ )
        {
            uint16_t dePos = 7+(i*10);
            if(read16(dePos+4) > 4)
            {
                uint32_t p = read32(dePos+6);
                if(p < posFreeData)
                {
                    //Now remember the pos before him
                    posFreeData = p;
                }
            }
        }
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
    posFreeData = EEPROM_MAX_SIZE;
}

uint16_t FT_EDS::getDEC()
{
    return read16(5);
}

bool FT_EDS::updateDE(edsId id, edsType type, uint8_t* data, unsigned int len)
{
    unsigned int pos = 0;
    uint16_t dec = getDEC();
    for( int i = 0 ; i < dec ; i++ )
    {
        pos = 7+(i*10);
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

        write16(posNextDE,   (uint16_t)id);   //deId
        write16(posNextDE+2, (uint16_t)type); //deType
        write16(posNextDE+4, (uint16_t)len);  //deLen

        if(len <= 4)
        {
            //data fits in the area
            for( unsigned int i=0 ; i<len ; i++ )
            {
                EEPROM.write(posNextDE+6+i, data[i]);
            }
        }
        else
        {
            //data goes into the free area, deData has a pointer to it!
            posFreeData -= len;
            write32(posNextDE+6, (uint32_t)(posFreeData));  //deLen

            for( unsigned int i=0 ; i<len ; i++ )
            {
                EEPROM.write(posFreeData+i, data[i]);
            }
        }
        posNextDE += 10;
    }
    else
    {
        //update the old DE found at "pos"
        /// @todo check that old type is the same as the new type!
        /// @todo check that old len is the same as the new len!
        write16(pos+2, (uint16_t)type); //deType
        write16(pos+4, (uint16_t)len);  //deLen
        if(len > 4)
        {
            uint32_t p = read32(pos+6);
            if(p>EEPROM_MAX_SIZE)
            {
                //Where did this pointer go???
                return false;
            }
            for( unsigned int i=0 ; i<len ; i++ )
            {
                EEPROM.write(p+i, data[i]);
            }
        }
        else
        {
            for( unsigned int i=0 ; i<len ; i++ )
            {
                EEPROM.write(pos+6+i, data[i]);
            }

        }
    }

    return true;
}


bool FT_EDS::readDE(edsId id, edsType type, uint8_t* data, unsigned int len)
{
    unsigned int pos = 0;
    uint16_t dec = getDEC();
    for( int i = 0 ; i < dec ; i++ )
    {
        pos = 7+(i*10);
        if(
                read16(pos)  ==(uint16_t)id &&
                read16(pos+2)==(uint16_t)type &&
                read16(pos+4)==(uint16_t)len
          )
        {
            if(len > 4)
            {
                uint32_t p = read32(pos+6);
                if(p>EEPROM_MAX_SIZE)
                {
                    //Where did this pointer go???
                    return false;
                }

                for( int i=0 ; i<len ; i++ )
                {
                    data[i] = EEPROM.read(p+i);
                }
            }
            else
            {
                for( int i=0 ; i<len ; i++ )
                {
                    data[i] = EEPROM.read(pos+6+i);
                }

            }
            return true;
        }
    }
    return false;
}


/**
 * How much space is free on the EEPROM?
 *
 * @return how many bytes is free
 */
unsigned int FT_EDS::free()
{
    return posFreeData - posNextDE;
}
