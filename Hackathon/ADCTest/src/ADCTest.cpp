#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Esp32MQTTClient.h"
#include <ArduinoJson.h>

using namespace std;

#define OLED_ADDR 0x3C      // OLED display address (for the 128x32)
#define RESET_PIN -1        // no reset pin
#define ANALOG_PIN 36       // analog input pin
#define MAX_VOLTAGE 1.0     // pin voltage
#define ADC_RESOLUTION 4095 // 12-Bit ADC
#define LED_PIN 2
#define BUTTON_PIN 4
#define BUZZER_PIN 5
//#define MOISTURE_PIN 15

// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(RESET_PIN);  

int analog_value = 0;
float value = 0.0;
string voltage = "";
bool led = false;
double threshold = 0;
int buttonState = 0;
bool switcher = true;

int freq = 2000;
int channel = 0;
int resolution = 8;

// SSID and password of WiFi
const char* ssid     = "FabLab";
const char* password = "wearemakers";

/* string containing Hostname, Device Id & Device Key in the format:                        */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "HostName=HAF-iot-hub.azure-devices.net;DeviceId=HAFESP32;SharedAccessKey=m+iHTeR1Q70rlMIxg+HEqlEMyyiXcIxSRlDCyypXmfw=";

static bool hasIoTHub = false;

double GetThreshold(int id)
{
    double threshold = 0;

    // query user profile via REST
    char request[1000] = {0};
    sprintf(request, "https://hafhealthprofiles.table.core.windows.net/HealthProfiles?$filter=ID%%20eq%%20%d&sv=2017-11-09&ss=t&srt=sco&sp=rwdlacu&se=2018-10-04T22:24:38Z&st=2018-10-03T14:24:38Z&spr=https,http&sig=l3E4RgW%%2BdMVdpzZooBguOumfI1OcFtYwubCI0bib5Rk%%3D", id);
    Serial.println(request);
    
    HTTPClient http;
    http.begin(request);
    http.addHeader("Accept", "application/json;odata=nometadata");
    http.addHeader("Content-Type", "application/json;odata=nometadata");
    int httpCode = http.GET();

    // get response
    if (httpCode > 0)
    { 
        String response = http.getString();
        Serial.println(httpCode);
        Serial.println(response);

        // parse JSON response
        //string test = "{\"value\":[{\"PartitionKey\":\"0\",\"RowKey\":\"1\",\"Timestamp\":\"2018-10-04T07:33:24.1054411Z\",\"AGE\":36,\"BMI\":29,\"FATIGUE\":0,\"ID\":2,\"LACTATE\":3,\"NAME\":\"Worker2\",\"PULS\":85,\"THRESHOLD\":0.5,\"WORKINTENSITY\":2}]}";
        StaticJsonBuffer<1000> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(response.c_str());

        if (!root.success())
        {
            Serial.println("parseObject() failed");
            return 0;
        }

        threshold = root["value"][0]["THRESHOLD"];
        Serial.println(threshold);

        // set worker name
        //http.begin("https://hafhealthprofiles.table.core.windows.net/ActiveWorker(PartitionKey='',RowKey='')?&sv=2017-11-09&ss=t&srt=sco&sp=rwdlacu&se=2018-10-04T22:24:38Z&st=2018-10-03T14:24:38Z&spr=https,http&sig=l3E4RgW%2BdMVdpzZooBguOumfI1OcFtYwubCI0bib5Rk%3D");
        http.addHeader("Content-Type", "text/plain");
        char data[1000] = {0};
        // sprintf(data, "{\"NAME\":\"%s\"}", (const char*)root["value"][0]["NAME"]);
        // Serial.println(data);
        // httpCode = http.PUT("{\"NAME\":\"Worker20\"}");

        sprintf(data, "https://www.fit.vutbr.cz/~ifiedor/hackathon/switch_active_user.php?user=%s", (const char*)root["value"][0]["NAME"]);
        http.begin(data);
        httpCode = http.GET();

        Serial.println(httpCode);
    }
    else
    {
        Serial.println("Error on HTTP request.");
    }

    // free the resources
    http.end();

    return threshold;
}

void setup()
{
    Serial.begin(115200);

    // initialize and clear display       
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    //pinMode(MOISTURE_PIN, INPUT);

    Serial.println("Starting connecting WiFi.");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    if (!Esp32MQTTClient_Init((const uint8_t*)connectionString))
    {
        Serial.println("Initializing IoT hub failed.");
        return;
    }
    hasIoTHub = true;

    threshold = GetThreshold(2);

    ledcSetup(channel, freq, resolution);
    ledcAttachPin(BUZZER_PIN, channel);
}

void loop()
{
    // ADC
    analog_value = analogRead(ANALOG_PIN);
    value = analog_value * MAX_VOLTAGE / ADC_RESOLUTION;
    voltage = to_string(value);
    Serial.println(voltage.c_str());
    
    // OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println(voltage.c_str());
    display.display();

    // change user
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH)
    {   if (switcher)
        {
            threshold = GetThreshold(1);
            switcher = false;
        }
        else
        {
            threshold = GetThreshold(2);
            switcher = true;
        }
    }

    if (value > MAX_VOLTAGE * threshold)
    {
        led = !led;
        digitalWrite(LED_PIN, led);

        // alarm on
        ledcWriteTone(channel, 1000);
    }
    else
    {
        digitalWrite(LED_PIN, LOW);

        // alarm off
        ledcWriteTone(channel, 0);
    }

    // Azure
    Serial.println("Start sending data.");
    if (hasIoTHub)
    {
        char buff[128];

        // data sent to Azure IoTHub
        snprintf(buff, 128, "{\"value\":%f, \"min\":0.0, \"max\":1.0, \"threshold\":%f}", value, threshold);

        if (Esp32MQTTClient_SendEvent(buff))
        {
            Serial.println("Sending data succeed.");
        }
        else
        {
            Serial.println("Failure...");
        }
    }

    // moisture
    // int moisture= digitalRead(MOISTURE_PIN);
    // Serial.println(moisture);
    // moisture = map(moisture,550,0,0,100);  
    // Serial.print("Mositure : ");
    // Serial.print(moisture);
    // Serial.println("%");

    delay(500);
}
