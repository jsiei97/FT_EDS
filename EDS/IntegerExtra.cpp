/**
 * @file IntegerExtra.cpp
 * @author Johan Simonsson
 * @brief Integer helper functions
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

#include "IntegerExtra.h"
#include <stdlib.h>
#include <string.h>
//#include <math.h>

/**
 * Converts a string with a unsigned int to a unsigned int.
 *
 * If the string begins with 0x then it will be parsed as hex,
 * all other cases will be parsed as dec.
 *
 * @param str The string that will be converted
 * @return the result, will return 0 on fail.
 */
unsigned int IntegerExtra::hex2uint(char* str)
{
    if(str == NULL)
        return 0;

    unsigned int num = 0;

    unsigned int size = strlen(str);
    if(size >= 3)
    {
        if('0'==str[0] && 'x'==str[1])
        {
            char* tmp = &str[2];
            //printf("%s:%d %s \n", __func__, __LINE__, tmp);


            int pos=(strlen(tmp)-1);
            int cnt=0;
            for( ; pos>=0 ; pos-- )
            {
                unsigned int scale = (1<<(4*cnt++));
                //printf("%s%d - %d %d %c \n", __func__, __LINE__, pos, scale, tmp[pos]);

                if (tmp[pos] >= '0' && tmp[pos] <= '9')
                {
                    num += ( (tmp[pos] - '0')*scale);
                }
                else if (tmp[pos] >= 'a' && tmp[pos] <= 'f')
                {
                    num += ( (tmp[pos] - 'a' + 10)*scale);
                }
                else if (tmp[pos] >= 'A' && tmp[pos] <= 'F')
                {
                    num += ( (tmp[pos] - 'A' + 10)*scale );
                }
                else
                {
                    //Non valid char, just fail and return 0!
                    return 0;
                }
            }
        }
    }


    if(0==num)
        num = atoi(str);

    return num;
}
