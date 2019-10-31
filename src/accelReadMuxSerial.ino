//#include <SD.h>
#include <SPI.h>
#include "Wire.h"
#include "I2Cdev.h"
//#include "I2C.h"
#include "ADXL345.h"

//const int chipSelect = BUILTIN_SDCARD;
ADXL345 accel;
uint8_t buffer[6];

int16_t ax, ay, az;
//Mux control pins
int s0 = 5;
int s1 = 6;
int s2 = 7;
int s3 = 8;

int controlPin[] = {s0, s1, s2, s3};
int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };


//Mux in "SIG" pin
//int SIG_pin = 0;

void setup()
{
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Wire.begin();
  
  Serial.begin(57600);
for(int j = 0; j < 16; j++){
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[j][i]);
  }
  delay(1);
  I2Cdev::writeByte(0x53,0x31,ADXL345_RANGE_8G); //GMG put the ADXL345 into +/- 8G range by writing the value 0x02 to the DATA_FORMAT register.
  delay(1);

  //read the value at the SIG pin
  //int val = analogRead(SIG_pin);
  I2Cdev::writeByte(0x53, ADXL345_RA_POWER_CTL, 0); // reset all power settings
  delay(1);
  I2Cdev::writeByte(0x53, ADXL345_RA_POWER_CTL, 0x08); // reset all power settings
  delay(1);
//  setMeasureEnabled(true);
 // accel.initialize();
}
 //     I2Cdev::writeByte(devAddr, ADXL345_RA_POWER_CTL, 0); // reset all power settings
 //   
  
 /* 
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");*/
   
}

void loop()
{
 // accel.getAcceleration(&ax, &ay, &az);
  String dataString = "";

  for(int i = 0; i < 4; i ++){
    dataString = dataString + readMux(i);
  }
  dataString = dataString + ' ' + String(millis());
  Serial.println(dataString);
 
}

String readMux(int channel){
  String indiAccel = "";
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  delayMicroseconds(100);
  //read the value at the SIG pin
  //int val = analogRead(SIG_pin);
//  accel.initialize();
 // accel.getAcceleration(&ax, &ay, &az);

//
    I2Cdev::readBytes(0x53, ADXL345_RA_DATAX0, 6, buffer);
    ax = (int16_t)(buffer[1] << 8 | buffer[0]);
    ay = (int16_t)(buffer[3] << 8 | buffer[2]);
    az = (int16_t)(buffer[5] << 8 | buffer[4]);  

  delayMicroseconds(100);
//  uint8_t buff[6];
  //const int num_accl = num_distrib_accels;
//  Serial.println(ay);
//  Serial.println(az);
//  Serial.println(millis());
  
//  indiAccel = "ax" + String(channel) + '=' + String(ax) + ',' + "ay" + String(channel) + '='  + String(ay)+ ',' + "az" + String(channel) + '=' + String(az) + ',';
   indiAccel = String(ax) + ' ' + String(ay)+ ' ' + String(az) + ' '  + ' ' + ' ';
  //return the value
  return indiAccel;
}

