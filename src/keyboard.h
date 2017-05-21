#ifndef BLEKEYBOARD_H
#define BLEKEYBOARD_H
#include <CurieBLE.h>
#include "keyCode.h"

class BLEKeyboard:public Print{
  public:
    BLEKeyboard();

    void begin(BLEPeripheral &BLEPer);
    size_t write(uint8_t k);
    size_t press(uint8_t k);
    size_t release(uint8_t k);
    void releaseAll(void);
  private:
    uint8_t KeyboardReport[8];
    BLECharacteristic* Report;
    BLECharacteristic* HIDInformation;
    BLECharacteristic* ReportMap;
    BLECharacteristic* BootKeyboardInputReport;
    BLEUnsignedCharCharacteristic* HIDControlPoint;
    BLEUnsignedCharCharacteristic* ProtocolMode;
    //BLEUnsignedCharCharacteristic* ReportOutput;
    BLEUnsignedCharCharacteristic* BootKeyboardOutputReport;
    void sendReport();

};

#endif
