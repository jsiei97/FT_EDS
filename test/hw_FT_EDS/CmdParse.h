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

#ifndef  __CMD_PARSE_H
#define  __CMD_PARSE_H

typedef void (*cmd_func)(char*);

typedef enum StateInData
{
    STATE_RESET = 1,
    STATE_CMD,
    STATE_DATA
} Cmd_state_t;

typedef struct CMD_Struct {
    char* name;
    cmd_func cmd;
} Cmd_t;

#ifndef BUFF_MAX_SIZE
#define BUFF_MAX_SIZE 40
#endif

#ifndef CMD_MAX_CNT
#define CMD_MAX_CNT 10
#endif

class CmdParse
{
    private:
        char buff[BUFF_MAX_SIZE];
        unsigned int buff_pos;
        Cmd_t cmds[CMD_MAX_CNT];
        cmd_func currentCmd;
        Cmd_state_t state;
    public:
        void cmd_init();
        void cmd_add_char(char ch);
        void cmd_buff_print();
        void cmd_add_cmd(char* name, cmd_func cmd);
};

#endif  // __CMD_PARSE_H
