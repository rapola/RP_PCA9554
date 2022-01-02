
#include "src/RP_PCA9554.h"

//device address
const uint8_t PCA9554addr = 0x24;

//object
PCA9554 U4;


void setup() {
  Serial.begin(115200);

  //initialize device and print state
  uint8_t a = U4.init(PCA9554addr);
  Serial.print("PCA-init-code: ");
  Serial.println(a);

  //set buffer for four pins as input, not sent to device yet...
  U4.pinModeBuf(0, INPUT);
  U4.pinModeBuf(1, INPUT);
  U4.pinModeBuf(2, INPUT);
  U4.pinModeBuf(3, INPUT);
  //send the buffer to device
  U4.pinModeUpdate();

  //set four pins as output, every change is sent immediately to device..
  U4.pinMode(4, OUTPUT);
  U4.pinMode(5, OUTPUT);
  U4.pinMode(6, OUTPUT);
  U4.pinMode(7, OUTPUT);
  
  //set pins in buffer high, not sent to device yet...
  U4.digitalWriteBuf(4, HIGH);
  U4.digitalWriteBuf(5, HIGH);
  U4.digitalWriteBuf(6, HIGH);
  U4.digitalWriteBuf(7, HIGH);
  //send the buffer to device
  U4.writeBufUpdate();
  
  delay(2000);

  //set pins in buffer low, not sent to device yet...
  U4.digitalWriteBuf(4, LOW);
  U4.digitalWriteBuf(5, LOW);
  U4.digitalWriteBuf(6, LOW);
  U4.digitalWriteBuf(7, LOW);
  //send the buffer to device
  U4.writeBufUpdate();
  delay(2000);

  //set pins immediately...
  U4.pinMode(0, OUTPUT);
  U4.digitalWrite(0, HIGH);
  U4.digitalWrite(7, HIGH);
  delay(2000);
  U4.digitalWrite(0, LOW);
  U4.digitalWrite(7, LOW);
 
}

void loop() {
  //read all pins from device to input buffer and print it...
  U4.readBufUpdate();
  Serial.print("Read PortA: ");
  Serial.println(U4.readPortA());

  //read input and set output immediately...
  uint8_t b = U4.digitalRead(0);
  U4.digitalWrite(1, b);

  delay(500); 

}
