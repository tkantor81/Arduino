#include <string>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TM1637Display.h>

#include "Portfolio.h"
#include "Finnhub.h"
#include "Marketstack.h"
#include "Alphavantage.h"

using namespace std;

#define BAUD_RATE     115200
#define SSID          "ralph_net"
#define PSSWD         "u6perdun"
#define OLED_ADDR     0x3C // OLED display address (for the 128x32)
#define SCREEN_WIDTH  128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32   // OLED display height, in pixels
#define RESET_PIN     -1   // no reset pin
#define CLK_PIN       18   // 4-digit seven segment display CLK pin
#define DIO_PIN       19   // 4-digit seven segment display DIO pin

// reset pin not used on 4-pin OLED module
// declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 infoDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN);
// set up the 4-digit seven segment display
TM1637Display profitDisplay(CLK_PIN, DIO_PIN);

Portfolio portfolio;
//Finnhub finnhub;
//Marketstack marketstack;
Alphavantage alphavantage;

void PrintToInfoDisplay(const string message)
{
    infoDisplay.clearDisplay();
    infoDisplay.setCursor(0, 0);
    infoDisplay.setTextSize(1);
    infoDisplay.setTextColor(WHITE);
    infoDisplay.println(message.c_str());
    infoDisplay.display();
}

void setup()   
{         
    Serial.begin(BAUD_RATE);

    // initialize OLED display
    if(!infoDisplay.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println("OLED display allocation failed");
    }

    // set the seven segment display to maximum brightness
    profitDisplay.setBrightness(0x0a);

    Serial.print("\nConnecting to ");
    Serial.println(SSID);
    
    WiFi.begin(SSID, PSSWD);
    while (WiFi.status() != WL_CONNECTED) // check the current connection status
    {
        delay(50);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    PrintToInfoDisplay("WiFi connected");
}

const float USD_HKD = 7.75; // USD/HKD 12.8.2020
const float USD_EUR = 0.85; // USD/EUR 12.8.2020

void loop() 
{
    if (WiFi.status() == WL_CONNECTED)
    {
        try
        {
            float totalInvested = 0;
            float totalValue = 0;
            
            list<Stock> stocks = portfolio.GetStocks();
            for (Stock& stock : stocks)
            {
                totalInvested += stock.invested;

                float value = alphavantage.GetEndOfDayPrice(stock.symbol) * stock.units;
                if (stock.exchange == "SEHK")
                {
                    value /= USD_HKD;
                }
                else if (stock.exchange == "Euronext Paris")
                {
                    value /= USD_EUR;
                }
                totalValue += value;
            }

            Serial.print("Total invested: ");
            Serial.println(totalInvested);
            Serial.print("Total value: ");
            Serial.println(totalValue);
            profitDisplay.showNumberDec(totalValue - totalInvested);
        }
        catch (const std::exception& e)
        {
            Serial.println(e.what());
            PrintToInfoDisplay(e.what());
        }
    }
    else
    {
        Serial.println("WiFi disconnected");
        PrintToInfoDisplay("WiFi disconnected");
        profitDisplay.showNumberDec(0);
    }

    delay(3600000); // 1 hour
}