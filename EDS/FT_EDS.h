/**
 * @file FT_EDS.h
 * @author Johan Simonsson  
 * @brief The Fun Tech EEPROM Data Storage class
 */


#include <inttypes.h>

#define MAGIC_0 0x23
#define MAGIC_1 0x45
#define MAGIC_2 0x54
#define MAGIC_3 0x32

#define FT_EDS_REV 1

#ifndef EEPROM_MAX_SIZE
#define EEPROM_MAX_SIZE 1024
#endif

typedef enum eds_ID
{
    EDS_ETH_MAC = 1,
    EDS_ONEWIRE_LIST,
} edsId;

typedef enum eds_TYPE
{
    EDS_BYTE = 1,  ///< uint8_t
    EDS_WORD,      ///< uint32_t
    //EDS_SWORD,     ///< int32_t
    EDS_BYTE_ARRAY,///< Array with uint8_t
} edsType;

class FT_EDS 
{
     private: 
         uint16_t read16(int address);
         uint32_t read32(int address);
         void write16(int address, uint16_t data);
         void write32(int address, uint32_t data);

         unsigned int posNextDE;
         unsigned int posFreeData;

     public: 
         void init();
         uint16_t getDEC();

         bool updateDE(edsId id, edsType type, uint8_t* data, unsigned int len);
         bool readDE  (edsId id, edsType type, uint8_t* data, unsigned int len);

         unsigned int getFree();
};
