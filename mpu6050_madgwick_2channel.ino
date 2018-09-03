//The code corresponding to readings from two IMUs belongs to Shuvashish Sarker, I've only rewritten it the way I needed.
//https://gitlab.com/shuvashish/batikkrom.com/blob/master/MuttipleMPU/MuttipleMPU.ino
//Code related to Madgwick filter is taken from its Example provided with Arduino IDE.

//Couple of advices:
//This code cannot be directly expanded to have more MPU6050s than two. 
//To have more MPU6050s, conisder bying a multiplexer and adress 
//all the MPU6050s with the same adress but multiplexe the SDA channel

//Attention: MPU6050 has no magnetometer, so the yaw (heading) readings suffer a large drift.
//In fact, it is a 5-reliable-axis IMU.

//If this implementation doesn't suit you for some reason, take a look at a this:
//https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

#include <Wire.h>
#include <MadgwickAHRS.h>

Madgwick filter1, filter2;
unsigned long microsPerReading, microsPrevious;

const int MPU2 = 0x69, MPU1=0x68;

void setup(){
  
  connectMPU6050(MPU1);
  connectMPU6050(MPU2);

  Serial.begin(38400);

  filter1.begin(25);
  filter2.begin(25);
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
  
}

void loop(){
  float acc1[3], gyr1[3];
  float acc2[3], gyr2[3];
  float rpy1[3], rpy2[3];
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
  
    GetMpuValue(MPU2, acc2, gyr2);
//    Serial.print("Acc 2:"); Serial.print("\t");
//    Serial.print(acc2[0]); Serial.print("\t");
//    Serial.print(acc2[1]); Serial.print("\t");
//    Serial.print(acc2[2]); Serial.print("\t");
//    Serial.print("Gyr 2:"); Serial.print("\t");
//    Serial.print(gyr2[0]); Serial.print("\t");
//    Serial.print(gyr2[1]); Serial.print("\t");
//    Serial.print(gyr2[2]); Serial.println("\t");

    filter1.updateIMU(gyr1[0], gyr1[1], gyr1[2], acc1[0], acc1[1], acc1[2]);
    filter2.updateIMU(gyr2[0], gyr2[1], gyr2[2], acc2[0], acc2[1], acc2[2]);
    
    rpy1[0] = filter1.getRoll();
    rpy1[1] = filter1.getPitch();
    rpy1[2] = filter1.getYaw();
    rpy2[0] = filter2.getRoll();
    rpy2[1] = filter2.getPitch();
    rpy2[2] = filter2.getYaw();

    // print the roll, pitch and yaw
    Serial.print("Or-n 1: ");
    Serial.print(rpy1[0]); Serial.print(" ");
    Serial.print(rpy1[1]); Serial.print(" ");
    Serial.print(rpy1[2]); Serial.print(" ");

    Serial.print("Or-n 2: ");
    Serial.print(rpy2[0]); Serial.print(" ");
    Serial.print(rpy2[1]); Serial.print(" ");
    Serial.println(rpy2[2]); 

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

