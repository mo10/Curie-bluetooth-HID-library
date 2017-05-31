#include "mouse.h"

unsigned char  MouseReportDescriptor[] = {
  //通用桌面设备
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  //鼠标
  0x09, 0x02, // USAGE (Mouse)
  //集合
  0xa1, 0x01, // COLLECTION (Application)
  //指针设备
  0x09, 0x01, // USAGE (Pointer)
  //集合
  0xa1, 0x00, // COLLECTION (Physical)
  //按键
  0x05, 0x09, // USAGE_PAGE (Button)
  //使用最小值1
  0x19, 0x01, // USAGE_MINIMUM (Button 1)
  //使用最大值3。1表示左键，2表示右键，3表示中键
  0x29, 0x03, // USAGE_MAXIMUM (Button 3)
  //逻辑最小值0
  0x15, 0x00, // LOGICAL_MINIMUM (0)
  //逻辑最大值1
  0x25, 0x01, // LOGICAL_MAXIMUM (1)
  //数量为3
  0x95, 0x03, // REPORT_COUNT (3)
  //大小为1bit
  0x75, 0x01, // REPORT_SIZE (1)
  //输入，变量，数值，绝对值
  //以上3个bit分别表示鼠标的三个按键情况，最低位（bit-0）为左键
  //bit-1为右键，bit-2为中键，按下时对应的位值为1，释放时对应的值为0
  0x81, 0x02, // INPUT (Data,Var,Abs)
  //填充5个bit，补足一个字节
  0x95, 0x01, // REPORT_COUNT (1)
  0x75, 0x05, // REPORT_SIZE (5)
  0x81, 0x03, // INPUT (Cnst,Var,Abs)
  //用途页为通用桌面
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  //用途为X
  0x09, 0x30, // USAGE (X)
  //用途为Y
  0x09, 0x31, // USAGE (Y)
  //用途为滚轮
  0x09, 0x38, // USAGE (Wheel)
  //逻辑最小值为-127
  0x15, 0x81, // LOGICAL_MINIMUM (-127)
  //逻辑最大值为+127
  0x25, 0x7f, // LOGICAL_MAXIMUM (127)
  //大小为8个bits
  0x75, 0x08, // REPORT_SIZE (8)
  //数量为3个，即分别代表x,y,滚轮
  0x95, 0x03, // REPORT_COUNT (3)
  //输入，变量，值，相对值
  0x81, 0x06, // INPUT (Data,Var,Rel)
  //关集合
  0xc0, // END_COLLECTION
  0xc0 // END_COLLECTION
 };
 const uint8_t HIDInfo[4]={
   0x11,
   0x01,
   0x00,
   0x01
 };
BLEMouse::BLEMouse(){
  HIDInformation=new BLECharacteristic("2A4A", BLERead,sizeof(HIDInfo));
  ProtocolMode=new BLEUnsignedCharCharacteristic("2A4E",  BLERead | BLEWriteWithoutResponse);
  HIDControlPoint=new BLEUnsignedCharCharacteristic("2A4C", BLEWriteWithoutResponse);
  ReportMap=new BLECharacteristic("2A4B", BLERead,sizeof(MouseReportDescriptor));
  Report=new BLECharacteristic("2A4D", BLERead | BLENotify,4);
  //ReportID2=new BLECharacteristic("2A4D", BLENotify,3);
  BootMouseInputReport=new BLECharacteristic("2A33",BLERead |  BLENotify,4);

  memset(MouseReport, 0, sizeof(MouseReport));
}
void BLEMouse::begin(BLEPeripheral &BLEPer){
  /*
  Set Appearance as Mouse

  But setAppearance does not work.
  Fix setAppearance:https://github.com/01org/corelibs-arduino101/pull/493
  About Appearance:https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml
  */
  BLEPer.setAppearance(962);
  // add service and characteristic:
  BLEPer.addAttribute(*Report);
  BLEPer.addAttribute(*HIDInformation);
  BLEPer.addAttribute(*ProtocolMode);
  BLEPer.addAttribute(*HIDControlPoint);
  BLEPer.addAttribute(*ReportMap);
  //BLEPer.addAttribute(*ReportID2);
  BLEPer.addAttribute(*BootMouseInputReport);

  ProtocolMode->setValue(1);
  ReportMap->setValue(MouseReportDescriptor,sizeof(MouseReportDescriptor));
  BootMouseInputReport->setValue(MouseReport,sizeof(MouseReport));
  HIDInformation->setValue(HIDInfo,sizeof(HIDInfo));
}
void BLEMouse::click(uint8_t b) {
  press(b);
  release();
}

void BLEMouse::move(signed char x, signed char y, signed char wheel) {
  delay(50);
  // send key code
    MouseReport[1] = x;
    MouseReport[2] = y;
    MouseReport[3] = wheel;
    sendReport();
}

void BLEMouse::press(uint8_t b) {
  MouseReport[0] |= b;
  move(0, 0, 0);
}

void BLEMouse::release() {
  memset(MouseReport, 0, sizeof(MouseReport));
  sendReport();
}

void BLEMouse::sendReport(){
  Report->setValue(MouseReport,sizeof(MouseReport));
}
bool BLEMouse::isPressed(uint8_t b) {
 return ((MouseReport[0] & b) != 0);
}
