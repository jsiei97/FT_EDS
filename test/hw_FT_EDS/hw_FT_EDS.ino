#include <EEPROM.h>
#include "FT_EDS.h"

FT_EDS eds;
int currentChar = 0;

#define BUFFER_SIZE 128
uint8_t buffer[BUFFER_SIZE];
int bufferPos;

void setup()
{
    Serial.begin(9600);
    Serial.println("FT EDS test!");
    eds.init();

    Serial.print  ( "DEC: " );
    Serial.println( eds.getDEC() );

    bufferPos = 0;
}

void loop()
{
    if (Serial.available() > 0)
	{
        currentChar = Serial.read();

        //Echo
        //Serial.print("Char: 0x");
        //Serial.println(currentChar, HEX);

        if(currentChar == '=')
        {
            //ToDo parse cmd
            //where chall I send the data?
            buffer[bufferPos] = '\0';
            Serial.print("CMD :");
            Serial.println((char*)buffer);
            bufferPos=0;
        }
        else if(currentChar == '\n')
        {
            //ToDo send data to valid function found
            //with parse cmd code...
            buffer[bufferPos] = '\0';
            Serial.print("Data:");
            Serial.println((char*)buffer);
            bufferPos=0;
        }
        else if(currentChar >= 0x21 && currentChar <= 0x7e)
        {
            //The printable 7bit ascii chars
            buffer[bufferPos] = currentChar;
            bufferPos++;
        }
        else
        {
            //Other chars, print hex code and ignore!
            Serial.print("0x");
            Serial.println(currentChar, HEX);
        }

        if(bufferPos == BUFFER_SIZE-1)
        {
            Serial.println("Overflow...");
            bufferPos = 0;
        }

        //Syntax CMD=DATA\n
        //if we get a '=' then parse and change state
        //-> select active cmd
        //-> reset, invalid cmd
        //if we get a '\n' then parse and send data to cmd


		//Send this char into a buffer
		//if '\n' then parse commmand
		//dump eeprom cmd
		//read data cmd
		//write data cmd (write payload)
    }
}
