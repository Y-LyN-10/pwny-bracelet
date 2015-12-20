/**
 * sketch_hopefully_final_final
 *
 *  Created on: 20.12.2015 11:00h
 *  
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>


#include <Wire.h>
#include "BMA250.h"
BMA250 accel;

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

 Serial.begin(9600);
  Wire.begin(2,14);
  accel.begin(BMA250_range_2g, BMA250_update_time_64ms);//This sets up the BMA250 accelerometer
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("HackBulgaria", "");

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;


        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("192.168.1.12", 443, "/test.html", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("10.0.203.231", 3001); //HTTP

        
accel.read();//This function gets new data from the accelerometer
//Serial.print("Incoming Data");
//http.POST("Incoming Data");
//Serial.print("X = ");
//http.POST("X = ");
int X = accel.X;
http.begin("10.0.203.231",3001, "X");
http.POST("X");
Serial.println(accel.X);

//  Serial.print("  ");
//  Serial.print("Y = ");
//  Serial.print(accel.Y);
//  Serial.print("  ");
//  Serial.print("Z = ");
//  Serial.print(accel.Z);
//  Serial.println();

//        USE_SERIAL.print("[HTTP] GET...\n");
//        // start connection and send HTTP header
//        int httpCode = http.GET();
//        if(httpCode) {
//            // HTTP header has been send and Server response header has been handled
//            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

//            // file found at server
//            if(httpCode == 200) {
//                String payload = http.getString();
//                USE_SERIAL.println(payload);
//           }
//        } else {
//            USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
//        }
    }

    delay(1000);
}

