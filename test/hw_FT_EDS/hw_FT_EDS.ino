#include <EEPROM.h>
#include "FT_EDS.h"
#include "CmdParse.h"

FT_EDS eds;
CmdParse cmdp;

int currentChar = 0;

#define BUFFER_SIZE 128
uint8_t buffer[BUFFER_SIZE];
int bufferPos;

void test(char* str)
{
    Serial.println( "Test function" );
    Serial.println( str );

}

void setup()
{
    Serial.begin(9600);
    Serial.println("FT EDS test!");
    eds.init();

    Serial.print  ( "DEC: " );
    Serial.println( eds.getDEC() );

    cmdp.cmd_init();
    cmdp.cmd_add_cmd("test", test);

    bufferPos = 0;
}

void loop()
{
    if (Serial.available() > 0)
    {
        cmdp.cmd_add_char(Serial.read());

        //Syntax CMD=DATA\n

        //todo add some cmds
        //dump eeprom cmd
        //read data cmd
        //write data cmd (write payload)
    }
}
