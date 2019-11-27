//The code corresponding to readings from two IMUs belongs to Shuvashish Sarker, I've only rewritten it the way I needed.
//https://gitlab.com/shuvashish/batikkrom.com/blob/master/MuttipleMPU/MuttipleMPU.ino
//Code related to Madgwick filter is taken from its Example provided with Arduino IDE.

// Works with MPU92/65 (MPU9250), the same connection: SCL A5, SDA A4, VCC +3.3V.

//If this implementation doesn't suit you for some reason, take a look at a this:
//https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

#include <Wire.h>
#include <MadgwickAHRS.h>

Madgwick filter1;
unsigned long microsPerReading, microsPrevious;

const int MPU1=0x68;

void setup(){
  
  connectMPU6050(MPU1);

  Serial.begin(38400);

  filter1.begin(25);
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
  
}

void loop(){
  float acc1[3], gyr1[3], rpy1[3];
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
    GetMpuValue(MPU1, acc1, gyr1);
//    Serial.print("Acc 1:"); Serial.print("\t");
//    Serial.print(acc1[0]); Serial.print("\t");
//    Serial.print(acc1[1]); Serial.print("\t");
//    Serial.print(acc1[2]); Serial.print("\t");
//    Serial.print("Gyr 1:"); Serial.print("\t");
//    Serial.print(gyr1[0]); Serial.print("\t");
//    Serial.print(gyr1[1]); Serial.print("\t");
//    Serial.print(gyr1[2]); Serial.print("\t");
  

    filter1.updateIMU(gyr1[0], gyr1[1], gyr1[2], acc1[0], acc1[1], acc1[2]);
    
    rpy1[0] = filter1.getRoll();
    rpy1[1] = filter1.getPitch();
    rpy1[2] = filter1.getYaw();
    
    // print the roll, pitch and yaw
    Serial.print("Or-n 1: ");
    Serial.print(rpy1[0]); Serial.print(" ");
    Serial.print(rpy1[1]); Serial.print(" ");
    Serial.print(rpy1[2]); Serial.print(" ");
    Serial.println("");

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }
}

void GetMpuValue(const int MPU, float * acc, float * gyr){
  
  Wire.beginTransmission(MPU); 
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU,6);
  while(Wire.available() < 6);

  acc[0] = ((float)(Wire.read()<<8|Wire.read()))/16384.0; 
  acc[1] = ((float)(Wire.read()<<8|Wire.read()))/16384.0;  
  acc[2] = ((float)(Wire.read()<<8|Wire.read()))/16384.0; 
  
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU,6);
  while(Wire.available() < 6);

  gyr[0] = ((float)(Wire.read()<<8|Wire.read()))/131.0; 
  gyr[1] = ((float)(Wire.read()<<8|Wire.read()))/131.0;  
  gyr[2] = ((float)(Wire.read()<<8|Wire.read()))/131.0; 

}

void connectMPU6050(int MPU){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();  
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission(); 
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission(); 
}
