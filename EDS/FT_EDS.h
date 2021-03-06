/**
 * @file FT_EDS.h
 * @author Johan Simonsson
 * @brief The Fun Tech EEPROM Data Storage class
 */

/*
 * Copyright (C) 2013 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef  __FT_EDS_H
#define  __FT_EDS_H

#include <inttypes.h>

/// Magic
#define MAGIC (0x23455432)

/// Data revision
#define FT_EDS_REV 2

#ifndef EEPROM_MAX_SIZE
/// Current eeprom size
#define EEPROM_MAX_SIZE 1024
#endif

/// Data for the eds id field
typedef enum eds_ID
{
    EDS_ETH_MAC      = 0x001, ///< Ethernet MAC for the Eth Shield

    EDS_ONEWIRE_LIST = 0x100, ///< List with 1w adresses.

    EDS_REGUL_P      = 0x200, ///< p in pid
    EDS_REGUL_I,              ///< i in pid
    EDS_REGUL_D,              ///< d in pid
} edsId;

/// Data for the eds type field
typedef enum eds_TYPE
{
    EDS_BYTE = 1,  ///< uint8_t
    EDS_WORD,      ///< uint32_t

    EDS_UINT_16 = 0x90,///< Integer type uint16_t
    EDS_UINT_32,       ///< Integer type uint32_t
    EDS_INT_16,        ///< Integer type int16_t
    EDS_INT_32,        ///< Integer type int32_t

    EDS_FIXED_32_16 = 0x100, ///< 32 bit fixed point with 2^16 scale
    EDS_FIXED_32_8,          ///< 32 bit fixed point with 2^8 scale
    EDS_FIXED_32_4,          ///< 32 bit fixed point with 2^4 scale

    EDS_BYTE_ARRAY = 0x200,  ///< Array with uint8_t
} edsType;

/**
 * A EEPROM Data Storage class
 */
class FT_EDS
{
    protected:
        uint16_t read16(int address);
        uint32_t read32(int address);
        void write16(int address, uint16_t data);
        void write32(int address, uint32_t data);

        unsigned int posNextDE;
        unsigned int posFreeData;

        unsigned int getPos(edsId id);

    public:
        void init();
        void format();
        uint16_t getDEC();

        bool readDE  (edsId id, edsType type, uint8_t* data, uint16_t len);
        bool updateDE(edsId id, edsType type, uint8_t* data, uint16_t len);

        bool readDE(edsId id, double* data);
        bool readDE(edsId id, int16_t* data);
        bool readDE(edsId id, int32_t* data);
        bool readDE(edsId id, uint16_t* data);
        bool readDE(edsId id, uint32_t* data);

        bool updateDE(edsId id, edsType type, double data);
        bool updateDE(edsId id, edsType type, int16_t data);
        bool updateDE(edsId id, edsType type, int32_t data);
        bool updateDE(edsId id, edsType type, uint16_t data);
        bool updateDE(edsId id, edsType type, uint32_t data);

        bool getDEInfo(unsigned int dePos, edsId* id, edsType* type, uint16_t* len);

        unsigned int getFree();
};

#endif  // __FT_EDS_H
