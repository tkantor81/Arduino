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
#define SSID          "vali_net_ext_2.4GHz"
#define PSSWD         "u6perdun"
#define OLED_ADDR     0x3C // OLED display address (for the 128x32)
#define SCREEN_WIDTH  128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32   // OLED display height, in pixels
#define RESET_PIN     -1   // no reset pin
#define CLK_PIN       18   // 4-digit seven segment display CLK pin
#define DIO_PIN       19   // 4-digit seven segment display DIO pin
#define LED_R_PIN     15   // RGB LED red pin
#define LED_G_PIN      2   // RGB LED green pin
#define LED_B_PIN      0   // RGB LED blue pin

// reset pin not used on 4-pin OLED module
// declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 infoDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN);
// set up the 4-digit seven segment display
TM1637Display profitDisplay(CLK_PIN, DIO_PIN);

void PrintToInfoDisplay(const string message)
{
    infoDisplay.clearDisplay();
    infoDisplay.setCursor(0, 0);
    infoDisplay.setTextSize(1);
    infoDisplay.setTextColor(WHITE);
    infoDisplay.println(message.c_str());
    infoDisplay.display();
}

void SetRGBLight(const int red, const int green, const int blue)
{
    digitalWrite(LED_R_PIN, red);
    digitalWrite(LED_G_PIN, green);
    digitalWrite(LED_B_PIN, blue);
}

void setup()   
{         
    Serial.begin(BAUD_RATE);

    // initialize the digital pin as an output for RGB LED
    pinMode(LED_R_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    SetRGBLight(LOW, LOW, HIGH);
    
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

const float USD_HKD = 7.75; // USD/HKD 30.9.2020
const float USD_EUR = 0.85; // USD/EUR 30.9.2020
float lastProfit = 0;

void loop() 
{
    if (WiFi.status() == WL_CONNECTED)
    {
        try
        {
            float totalInvested = 0;
            float totalValue = 0;

            Portfolio portfolio;
            Alphavantage alphavantage;
            
            list<Stock> stocks = portfolio.GetStocks();
            for (Stock& stock : stocks)
            {
                totalInvested += stock.invested;

                Serial.print("Stock requesting: ");
                Serial.println(stock.company.c_str()); 
                float value = alphavantage.GetEndOfDayPrice(stock.symbol) * stock.units;
                if (stock.exchange == "SEHK")
                {
                    value /= USD_HKD;
                }
                else if (stock.exchange == "Euronext Paris")
                {
                    value /= USD_EUR;
                }
                Serial.print("Value: ");
                Serial.println(value);
                totalValue += value;

                delay(15000); // 15 seconds (Alpha Vantage - up to 5 API requests per minute)
            }

            Serial.print("Total invested: ");
            Serial.println(totalInvested);
            Serial.print("Total value: ");
            Serial.println(totalValue);
            
            float profit = totalValue - totalInvested;
            profitDisplay.showNumberDec(profit);
            if (profit > lastProfit)
            {
                SetRGBLight(LOW, HIGH, LOW); // green light
            }
            else if (profit < lastProfit)
            {
                SetRGBLight(HIGH, LOW, LOW); // red light
            }
            lastProfit = profit;
        }
        catch (const std::exception& e)
        {
            Serial.println(e.what());
            PrintToInfoDisplay(e.what());
            SetRGBLight(LOW, LOW, HIGH);
        }
    }
    else
    {
        Serial.println("WiFi disconnected");
        PrintToInfoDisplay("WiFi disconnected");
        SetRGBLight(LOW, LOW, HIGH);
    }

    delay(1800000); // 0.5 hour (Alpha Vantage - up to 500 API requests per day)
}