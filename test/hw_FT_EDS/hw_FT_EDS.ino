#include <EEPROM.h>
#include "FT_EDS.h"
#include "CmdParse.h"

FT_EDS eds;
CmdParse cmdp;

int currentChar = 0;

#define BUFFER_SIZE 128
uint8_t buffer[BUFFER_SIZE];
int bufferPos;

void info(char* str)
{
    //Print all info on the EEPROM.
    Serial.println( "Info function" );
    //Serial.println( str );

    Serial.print  ( "Free: " );
    Serial.println( eds.getFree() );

    Serial.print  ( "DEC: " );
    Serial.println( eds.getDEC() );

    edsId id;
    edsType type;
    unsigned int size;
    for( unsigned int de=0 ; de<eds.getDEC() ; de++ )
    {
        if(eds.getDEInfo(de, &id, &type, &size))
        {
            Serial.print  ( "DE: " );
            Serial.print  ( de, DEC );
            Serial.print  ( ", id=" );
            Serial.print  ( id, HEX );
            Serial.print  ( ", type=" );
            Serial.print  ( type, HEX );
            Serial.print  ( ", size=" );
            Serial.println( size, DEC );
        }
    }
}

void format(char* str)
{
    Serial.println( "Format EEPROM" );
    //Serial.println( str );
    eds.format();
    Serial.println( "Done..." );
}

void put(char* str)
{
    if(str == NULL)
        return;

    size_t len = strlen(str);
    if(len <= 2)
        return;

    //parse str
    //id=data
    //check that it is a valid id, and then select type and size

}

void get(char* str)
{
    if(str == NULL)
        return;

    size_t len = strlen(str);
    if(len == 0)
        return;

    //parse str
    //id
    //check that it is a valid id, and then select correct read function
}

void setup()
{
    Serial.begin(9600);
    Serial.println();
    Serial.println("FT EDS test!");
    eds.init();

    cmdp.cmd_init();
    cmdp.cmd_add_cmd("info", info);
    cmdp.cmd_add_cmd("format", format);
    cmdp.cmd_add_cmd("put", put);
    cmdp.cmd_add_cmd("get", get);

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
