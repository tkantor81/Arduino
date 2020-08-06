#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C   // OLED display address (for the 128x32)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define RESET_PIN -1     // no reset pin 

// reset pin not used on 4-pin OLED module
// declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN);  

void setup()   
{         
    Serial.begin(115200);

    // initialize
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }

    delay(1000);
    display.clearDisplay();

    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("Hello,\nworld!");
    display.display();
}

void loop() 
{ 
}
