#ifndef BLEMOUSE_H
#define BLEMOUSE_H
#include <CurieBLE.h>
#include "keyCode.h"

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)

class BLEMouse{
  public:
    BLEMouse();
    void begin(BLEPeripheral &BLEPer);
    void click(uint8_t b = MOUSE_LEFT);
    void move(signed char x, signed char y, signed char wheel = 0);
    void press(uint8_t b = MOUSE_LEFT);   // press LEFT by default
    void release(); // release LEFT by default
    bool isPressed(uint8_t b = MOUSE_LEFT); // check LEFT by default

  private:
    uint8_t MouseReport[4];
    BLECharacteristic* Report;
    //BLECharacteristic* ReportID2;
    BLECharacteristic* HIDInformation;
    BLECharacteristic* ReportMap;
    BLECharacteristic* BootMouseInputReport;
    BLEUnsignedCharCharacteristic* HIDControlPoint;
    BLEUnsignedCharCharacteristic* ProtocolMode;
    void sendReport();

    unsigned char _button;
};

#endif
