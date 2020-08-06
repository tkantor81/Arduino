#include <string>
#include "Arduino.h"
#include <TM1637Display.h>

using namespace std;

#define CLK_PIN 18
#define DIO_PIN 19
 
 // set up the 4-digit display
TM1637Display display(CLK_PIN, DIO_PIN);

void setup()   
{         
    Serial.begin(115200);

     // set the diplay to maximum brightness
    display.setBrightness(0x0a);
}

void loop() 
{
    for (int i = 0; i < 10000; ++i)
    {
        display.showNumberDec(i);
        Serial.println(to_string(i).c_str());
        delay(1000);
    } 
}
