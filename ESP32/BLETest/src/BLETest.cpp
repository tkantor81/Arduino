#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C // OLED display address (for the 128x32)
#define RESET_PIN -1   // no reset pin

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// Reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(RESET_PIN);  

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer)
    {
        Serial.println("Device connected");
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer)
    {
        Serial.println("Device disconnected");
        deviceConnected = false;
    }
};

class MyCharacteristicCallbacks: public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();

        if (!rxValue.empty())
        {
            Serial.print("Received Value: ");
            Serial.println(rxValue.c_str());

            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.println(rxValue.c_str());
            display.display();
        }
    }
};

void setup()
{
    Serial.begin(115200);

    // Initialize and clear display       
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();

    // Create the BLE Device
    BLEDevice::init("ESP32");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristics
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,
                                                       BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,
                                                                          BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Start advertising...");
}

void loop()
{
    if (deviceConnected)
    {
        oldDeviceConnected = true;
        
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        Serial.println("Notification sent");
        
        txValue++;
        delay(1000); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // Disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        oldDeviceConnected = false;
        delay(500); // give the bluetooth stack the chance to get things ready
        
        // Restart advertising
        pServer->startAdvertising();
        Serial.println("Start advertising...");
    }
}
