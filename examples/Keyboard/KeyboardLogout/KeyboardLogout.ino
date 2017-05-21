/*
 Curie Bluetooth Keyboard Logout test

 For the Arduino/Genuino 101.

 When you connect pin 2 to ground, it performs a logout.
 It uses keyboard combinations to do this, as follows:

  On Windows, CTRL-ALT-DEL followed by ALT-l
  On Ubuntu, CTRL-ALT-DEL, and ENTER
  On OSX, CMD-SHIFT-q
  On Android, GUI-B ,Open Web Browser

 created 21 May 2017
 by Mo10

 https://github.com/mo10/Curie-bluetooth-HID-library
*/

#include <CurieBLE.h>
#include <keyboard.h>

#define OSX 0
#define WINDOWS 1
#define UBUNTU 2
#define ANDROID 3

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService mouse101("1812");  // 0x1812 = Human Interface Device
BLEKeyboard kbd;

// change this to match your platform:
int platform = ANDROID;

const long timeout = 10000;  // Wait 10s

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
    bool islogout=false;
    while (central.connected()) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= timeout && !islogout) {
        Serial.print("Sending, ");
        switch (platform) {
          case OSX:
            Serial.println("OSX");
            kbd.press(KEY_LEFT_GUI);
            // Shift-Q logs out:
            kbd.press(KEY_LEFT_SHIFT);
            kbd.press('Q');
            delay(100);
            kbd.releaseAll();
            // enter:
            kbd.write(KEY_RETURN);
            break;
          case WINDOWS:
            // CTRL-ALT-DEL:
            Serial.println("Windows");
            kbd.press(KEY_LEFT_CTRL);
            kbd.press(KEY_LEFT_ALT);
            kbd.press(KEY_DELETE);
            delay(100);
            kbd.releaseAll();
            //ALT-l:
            delay(2000);
            kbd.press(KEY_LEFT_ALT);
            kbd.press('l');
            kbd.releaseAll();
            break;
          case UBUNTU:
            // CTRL-ALT-DEL:
            Serial.println("Ubuntu");
            kbd.press(KEY_LEFT_CTRL);
            kbd.press(KEY_LEFT_ALT);
            kbd.press(KEY_DELETE);
            delay(1000);
            kbd.releaseAll();
            // Enter to confirm logout:
            kbd.write(KEY_RETURN);
            break;
          case ANDROID:
            // GUI + B Open Web Browser:
            Serial.println("Android");
            kbd.press(KEY_LEFT_GUI);
            kbd.press('b');
            delay(1000);
            kbd.releaseAll();
            break;
        }
        islogout=true;
      }
    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
