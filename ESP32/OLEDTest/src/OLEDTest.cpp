#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C // OLED display address (for the 128x32)
#define RESET_PIN -1   // no reset pin

// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(RESET_PIN);  

void setup()   
{         
    // initialize and clear display       
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();

    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("Hello,\nworld!");
    display.display();
}

void loop() 
{ 
}
