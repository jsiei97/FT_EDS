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

void FT_EDS::format()
{
    //Remove magic
    write32(0,0);
    //Remove DEC
    write16(5,0);

    init();
}

uint16_t FT_EDS::getDEC()
{
    return read16(5);
}

bool FT_EDS::updateDE(edsId id, edsType type, double data)
{
    int32_t fData = 0;

    switch ( type )
    {
        case EDS_FIXED_32_16:
            /// @bug Overflow on target!
            fData = (int32_t)(data*(1<<16));
            break;
        default:
            type = EDS_FIXED_32_8;
            //fall throu ok since 32_8 is default!
        case EDS_FIXED_32_8:
            fData = (int32_t)(data*(1<<8));
            break;
    }

    uint8_t arr[4] ;
    for( int i=3 ; i>=0 ; i-- )
    {
        arr[i] = 0xFF & fData;
        fData >>= 8;
    }

    return updateDE(id, type, &arr[0], 4);
}

bool FT_EDS::updateDE(edsId id, edsType type, uint8_t* data, uint16_t len)
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
        if(len > getFree())
        {
            return false;
        }
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

        //Check that it has the same size as the old
        uint16_t oldLen = read16(pos+4);
        unsigned int diff = len-oldLen;


        if((oldLen < len) && (len > 4))
        {
            if(diff > getFree())
            {
                return false;
            }

            //It is NOT the same size!
            uint32_t oldP = read32(pos+6);

            //The size is different but maybe we can fit it in...
            if(oldP != posFreeData)
            {
                //Only allow the last data to grow,
                //the others has data before or after!
                return false;
            }

            //Move back the pointer to get more space
            oldP -= diff;
            posFreeData -= diff;
            write32(pos+6, oldP);
        }

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

bool FT_EDS::readDE(edsId id, double* data)
{
    bool ret = false;
    unsigned int pos = 0;
    uint16_t dec = getDEC();
    for( int i = 0 ; i < dec ; i++ )
    {
        pos = 7+(i*10);
        if( read16(pos) == (uint16_t)id )
        {
            uint16_t type = read16(pos+2);
            //uint16_t len  = read16(pos+4);
            int32_t raw = (int32_t)read32(pos+6);

            switch ( type )
            {
                case EDS_FIXED_32_8:
                    *data = (double)raw;
                    *data /= (1<<8);
                    ret = true;
                    break;
                case EDS_FIXED_32_16:
                    *data = (double)raw;
                    *data /= (1<<16);
                    ret = true;
                    break;
            }
        }
    }
    return ret;
}

bool FT_EDS::readDE(edsId id, edsType type, uint8_t* data, uint16_t len)
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

                for( unsigned int i=0 ; i<len ; i++ )
                {
                    data[i] = EEPROM.read(p+i);
                }
            }
            else
            {
                for( unsigned int i=0 ; i<len ; i++ )
                {
                    data[i] = EEPROM.read(pos+6+i);
                }

            }
            return true;
        }
    }
    return false;
}

bool FT_EDS::getDEInfo(unsigned int dePos, edsId* id, edsType* type, uint16_t* len)
{
    int pos = 7+(dePos*10);
    *id   = (edsId)read16(pos);
    *type = (edsType)read16(pos+2);
    *len  = read16(pos+4);
    return true;
}

/**
 * How much space is free on the EEPROM?
 *
 * @return how many bytes is free
 */
unsigned int FT_EDS::getFree()
{
    return posFreeData - posNextDE;
}
