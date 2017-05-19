/*
 Curie Bluetooth Keyboard Message test

 For the Arduino/Genuino 101.

 Every 10 seconds send a text string.
 created 19 Oct 2017
 modified 27 Mar 2012
 by Mo10

 https://github.com/mo10/Curie-bluetooth-keyboard-library
*/

#include <CurieBLE.h>
#include <keyboard.h>

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService mouse101("1812");  // 0x1812 = Human Interface Device
BLEKeyboard kbd;

const long interval = 10000;  // Delay time 10s

void setup() {
  Serial.begin(9600);

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("Curie Keyboard");
  blePeripheral.setAdvertisedServiceUuid(mouse101.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(mouse101);
  // initialize keyboard service:
  kbd.begin(blePeripheral);

  // begin advertising BLE service:
  blePeripheral.begin();
}

void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    unsigned long previousMillis = millis();
    // while the central is still connected to peripheral:
    int counter=0;
    while (central.connected()) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        // save the last time
        previousMillis = currentMillis;
        Serial.println("Sending.");
        kbd.print("Hello ");
        kbd.println(central.address());
        kbd.print(++counter);
        kbd.println(" times.");
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
