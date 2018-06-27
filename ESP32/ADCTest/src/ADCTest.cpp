#include <string>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C // OLED display address (for the 128x32)
#define RESET_PIN -1   // no reset pin
#define ANALOG_PIN 36  // analog input pin

// see the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(RESET_PIN);  

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
int analog_value = 0;

class MyServerCallbacks: public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer)
    {
        deviceConnected = true;
        Serial.println("Device connected");
    }

    void onDisconnect(BLEServer* pServer)
    {
        deviceConnected = false;
        Serial.println("Device disconnected");
        
        delay(500); // give the bluetooth stack the chance to get things ready
        Serial.println("Start advertising...");
        pServer->startAdvertising();
    }
};

void setup()
{
    Serial.begin(115200);

    // initialize and clear display       
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();

    // create the BLE Device
    BLEDevice::init("ESP32");

    // create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // create a BLE Characteristics
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,
                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());

    // start the service
    pService->start();

    Serial.println("Start advertising...");
    pServer->getAdvertising()->start();
}

void loop()
{
    analog_value = analogRead(ANALOG_PIN);
    Serial.println(analog_value);
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println(analog_value);
    display.display();

    if (deviceConnected)
    {
        pTxCharacteristic->setValue(std::to_string(analog_value));
        pTxCharacteristic->notify();
        Serial.println("Notification sent");
	}

    delay(500);
}
