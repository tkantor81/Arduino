#include <Arduino.h>
// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

//custom boards may override default pin definitions with BLEPeripheral(PIN_REQ, PIN_RDY, PIN_RST)
BLEPeripheral blePeripheral = BLEPeripheral();
// create service
BLEService bleService = BLEService("19b10000e8f2537e4f6cd104768a1214");
// create characteristics
BLECharCharacteristic wCharacteristic = BLECharCharacteristic("19b10001e8f2537e4f6cd104768a1214", BLEWrite);
BLECharCharacteristic rCharacteristic = BLECharCharacteristic("19b10002e8f2537e4f6cd104768a1214", BLERead | BLENotify);
// create descriptors
BLEDescriptor cudDescriptor = BLEDescriptor("2901", "Voltage");
BLEDescriptor cccDescriptor = BLEDescriptor("2902", "");

uint8_t value = 0;

void blePeripheralConnected(BLECentral& central)
{
    // central connected event handler
    Serial.print("Connected event, central: ");
    Serial.println(central.address());
}

void blePeripheralDisconnected(BLECentral& central)
{
    // central disconnected event handler
    Serial.print("Disconnected event, central: ");
    Serial.println(central.address());
}

void characteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Characteristic event, writen: ");

    if (*characteristic.value() == 1)
    {
        Serial.println(F("LED on"));
        digitalWrite(LED_BUILTIN, LOW);
    } 
    else
    {
        Serial.println(F("LED off"));
        digitalWrite(LED_BUILTIN, HIGH);
    }
}

void setup()
{
    Serial.begin(9600);

    // set LED pin to output mode
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // set advertised local name and service UUID
    blePeripheral.setLocalName("nRF52");
    blePeripheral.setAdvertisedServiceUuid(bleService.uuid());

    // assign event handlers for connected, disconnected to peripheral
    blePeripheral.setEventHandler(BLEConnected, blePeripheralConnected);
    blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnected);
    
    // add service and characteristics
    blePeripheral.addAttribute(bleService);
    blePeripheral.addAttribute(wCharacteristic);

    blePeripheral.addAttribute(rCharacteristic);
    blePeripheral.addAttribute(cudDescriptor);
    blePeripheral.addAttribute(cccDescriptor);

    // assign event handlers for characteristic
    wCharacteristic.setEventHandler(BLEWritten, characteristicWritten);

    // begin initialization
    blePeripheral.begin();

    Serial.println("BLE LED Peripheral");
}

void loop()
{
    // poll peripheral
    blePeripheral.poll();

    rCharacteristic.setValue(value);
    value++;
    delay(1000);
}
