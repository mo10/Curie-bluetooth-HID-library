#include <CurieBLE.h>
#include <CurieIMU.h>
#include <mouse.h>


BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService mouse101("1812");  // 0x1812 = Human Interface Device
BLEMouse mouse;

void setup() {
  Serial.begin(9600);

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("Curie Mouse");
  blePeripheral.setAdvertisedServiceUuid(mouse101.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(mouse101);
  // initialize keyboard service:
  mouse.begin(blePeripheral);

  // begin advertising BLE service:
  blePeripheral.begin();
  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
}


void loop() {
  float ax, ay, az;   //scaled accelerometer values
  int bx=0,by=0;
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    while (central.connected()) {
        CurieIMU.readAccelerometerScaled(ax, ay, az);
        int x = int(ax*10)*5;
        int y = int(ay*10)*5;
        if(bx != x || by != y ){
          Serial.print("x:");
          Serial.print(x);
          Serial.print("y:");
          Serial.println(y);
          mouse.move(0-x,y,0);
          bx=x;by=y;
        }
    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
