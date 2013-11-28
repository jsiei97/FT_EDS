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
#define CMD_MAX_CNT 5
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
