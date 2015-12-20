/*
TinyDuino Accelerometer Demo
  
May 25 2014, by Ben Rose

This example code is in the public domain.

http://www.tiny-circuits.com

*/


#include <Wire.h>
#include "BMA250.h"


BMA250 accel;


void setup()
{
  Serial.begin(9600);
  Wire.begin(2,14);
  accel.begin(BMA250_range_2g, BMA250_update_time_64ms);//This sets up the BMA250 accelerometer
}

void loop()
{
  accel.read();//This function gets new data from the accelerometer
  Serial.print("X = ");
  Serial.print(accel.X);
  Serial.print("  ");
  Serial.print("Y = ");
  Serial.print(accel.Y);
  Serial.print("  ");
  Serial.print("Z = ");
  Serial.print(accel.Z);
  Serial.println();
//  Serial.print("  Temperature(C) = ");
//  Serial.println((accel.rawTemp*0.5)+24.0,1);
  delay(250);//We'll make sure we're over the 64ms update time set on the BMA250
}
