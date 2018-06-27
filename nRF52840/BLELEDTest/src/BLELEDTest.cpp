#include <Arduino.h>
// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

//custom boards may override default pin definitions with BLEPeripheral(PIN_REQ, PIN_RDY, PIN_RST)
BLEPeripheral blePeripheral = BLEPeripheral();
// create service
BLEService bleService = BLEService("19b10000e8f2537e4f6cd104768a1214");
// create characteristic
BLECharCharacteristic bleCharacteristic = BLECharCharacteristic("19b10001e8f2537e4f6cd104768a1214", BLERead | BLEWrite);

void setup()
{
    Serial.begin(9600);

    // set LED pin to output mode
    pinMode(LED_BUILTIN, OUTPUT);

    // set advertised local name and service UUID
    blePeripheral.setLocalName("nRF52840");
    blePeripheral.setAdvertisedServiceUuid(bleService.uuid());
    
    // add service and characteristic
    blePeripheral.addAttribute(bleService);
    blePeripheral.addAttribute(bleCharacteristic);

    // begin initialization
    blePeripheral.begin();

    Serial.println("BLE LED Peripheral");
}

void loop()
{
    BLECentral bleCentral = blePeripheral.central();

    if (bleCentral)
    {
        Serial.print("Central connected to peripheral: ");
        Serial.println(bleCentral.address());

        while (bleCentral.connected())
        {
            // central still connected to peripheral
            if (bleCharacteristic.written())
            {
                // central wrote new value to characteristic, update LED
                if (bleCharacteristic.value())
                {
                    Serial.println("LED on");
                    digitalWrite(LED_BUILTIN, HIGH);
                }
                else
                {
                    Serial.println("LED off");
                    digitalWrite(LED_BUILTIN, LOW);
                }
            }
        }

        Serial.print("Central disconnected: ");
        Serial.println(bleCentral.address());
    }
}

/*
// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

// define pins (varies per shield/board)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

// LED pin
#define LED_PIN   3

// create peripheral instance, see pinouts above
BLEPeripheral           blePeripheral        = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService              ledService           = BLEService("19b10000e8f2537e4f6cd104768a1214");

// create switch characteristic
BLECharCharacteristic   switchCharacteristic = BLECharCharacteristic("19b10001e8f2537e4f6cd104768a1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);
#if defined (__AVR_ATmega32U4__)
  delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
#endif

  // set LED pin to output mode
  pinMode(LED_PIN, OUTPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("LED");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("BLE LED Peripheral"));
}

void loop() {
  // poll peripheral
  blePeripheral.poll();
}

void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print(F("Characteristic event, writen: "));

  if (switchCharacteristic.value()) {
    Serial.println(F("LED on"));
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println(F("LED off"));
    digitalWrite(LED_PIN, LOW);
  }
}
*/