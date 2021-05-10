#include <Wire.h>   
#include <mpu9250.h>

float value[3];
float value1[3];
mpu9250 testsub1,testsub2;
void setup() {
  // put your setup code here, to run once:
Wire.begin();
  Serial.begin(115200);
  Serial.println("Here2");
 //mpu9250 testsub2(0x68,1);
 testsub1.ini(0x68,1);
 testsub2.ini(0x68,2);
}

void loop() {
  
testsub1.getvalues(value);
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(value[0], 2);
    Serial.print(", ");
    Serial.print(value[1], 2);
    //Serial.print(", ");
    //Serial.println(value[2], 2);
testsub2.getvalues(value1);
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(value1[0], 2);
    Serial.print(", ");
    Serial.print(value1[1], 2);
    Serial.print(", ");
    Serial.println(value1[2], 2);

}
