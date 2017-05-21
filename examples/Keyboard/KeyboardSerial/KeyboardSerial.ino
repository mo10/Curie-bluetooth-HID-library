/*
 Curie Bluetooth Keyboard Serial test

 For the Arduino/Genuino 101.

 Reads a byte from the serial port, sends a keystroke.

 created 21 May 2017
 by Mo10

 https://github.com/mo10/Curie-bluetooth-HID-library
*/

#include <CurieBLE.h>
#include <keyboard.h>

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService mouse101("1812");  // 0x1812 = Human Interface Device
BLEKeyboard kbd;

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
    while (central.connected()) {
      if (Serial.available() > 0) {
        // read incoming serial data:
        char inChar = Serial.read();
        // Type the ASCII value from what you received:
        kbd.write(inChar);
      }
    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
