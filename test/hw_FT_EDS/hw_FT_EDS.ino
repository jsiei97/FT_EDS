#include <EEPROM.h>
#include "FT_EDS.h"

void setup() {
    Serial.begin(9600);
    Serial.println("FT EDS test!");
}

void loop() {

    bool res = false;

    if (false == res)
    {
        Serial.println("Error: fail...?");
    }

    /*
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C (");
    Serial.print(reading);
    Serial.println(")");
    */

    delay(1000);
}
