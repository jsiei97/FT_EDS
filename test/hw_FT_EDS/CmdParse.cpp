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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CmdParse.h"

#include "Arduino.h"
#define DEBUG_SERIAL_OUT


void CmdParse::cmd_init()
{
    int i;
    for( i=0 ; i<CMD_MAX_CNT ; i++ )
    {
        cmds[i].name = NULL;
        cmds[i].cmd = NULL;
    }

    buff_pos=0;
    currentCmd = NULL;
    state = STATE_RESET;
}

void CmdParse::cmd_add_cmd(char* name, cmd_func cmd)
{
    int i;
    for( i=0 ; i<CMD_MAX_CNT ; i++ )
    {
        if(cmds[i].name == NULL)
        {
            //printf("%s:%d HIT %d\n", __func__, __LINE__, i);
            cmds[i].name = name;
            cmds[i].cmd = cmd;
            break;
        }
    }
}

void CmdParse::cmd_add_char(char ch)
{
    if(state == STATE_RESET)
    {
        state = STATE_CMD;
        currentCmd = NULL;
        buff_pos = 0;
#ifdef DEBUG_SERIAL_OUT
        Serial.println();
        Serial.print(":-) ");
#else
        printf(":-) ");
#endif
    }

    if('=' == ch && state != STATE_DATA)
    {
#ifdef DEBUG_SERIAL_OUT
        Serial.print(ch);
#endif
        buff[buff_pos] = '\0';
        if(buff_pos > 0)
        {
            int i;
            for( i=0 ; i<CMD_MAX_CNT ; i++ )
            {
                if(cmds[i].name != NULL)
                {
                    if(strcmp(cmds[i].name, buff)==0)
                    {
                        currentCmd = cmds[i].cmd;
                        state = STATE_DATA;
                        break;
                    }
                }
            }
        }
        if(currentCmd == NULL)
        {
#ifdef DEBUG_SERIAL_OUT
            Serial.println();
            Serial.println("Error: not a valid command...");
#endif
            state = STATE_RESET;
        }
        buff_pos = 0;

    }
    else if('\n'==ch)
    {
        buff[buff_pos] = '\0';
        if(currentCmd!=NULL)
        {
#ifdef DEBUG_SERIAL_OUT
            Serial.println();
#endif
            currentCmd(&buff[0]);
        }
        state = STATE_RESET;
    }
    else if(ch >= 0x21 && ch <= 0x7e)
    {
        buff[buff_pos] = ch;
        buff_pos++;
#ifdef DEBUG_SERIAL_OUT
        Serial.print(ch);
#endif
    }
#if 0
    else
    {
        //Other chars, print hex code and ignore!
#ifdef DEBUG_SERIAL_OUT
        Serial.print("0x");
        Serial.println(ch, HEX);
#endif
    }
#endif


    if(buff_pos == BUFF_MAX_SIZE)
    {
        state = STATE_RESET;
    }
}

void CmdParse::cmd_buff_print()
{
#ifdef DEBUG_SERIAL_OUT
    for( unsigned int i = 0 ; i<BUFF_MAX_SIZE ; i++ )
    {
        Serial.print(" 0x");
        Serial.print(buff[i], HEX);
        if((i+1)%8==0)
            Serial.println("");
    }
    Serial.println("");
#else
    unsigned int i;
    for( i = 0 ; i<BUFF_MAX_SIZE ; i++ )
    {
        printf("%02x ", buff[i]);
        if((i+1)%8==0)
            printf("\n");
    }
    printf("\n");
#endif
}

