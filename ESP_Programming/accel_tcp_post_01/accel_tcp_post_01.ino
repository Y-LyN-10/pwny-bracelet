/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <Arduino.h>

#include <Wire.h>
#include "BMA250.h"
BMA250 accel;

#define USE_SERIAL Serial

String row = "";

const char* ssid     = "Vinyl's Mobile Den";
const char* password = "DJ PON3 is on tour";

const char* host = "192.168.43.47";
const char* streamId   = "....................";
const char* privateKey = "....................";

float G[3];
float alpha = 0.9;

long timer_read_from_accel = 0;
long timer_send_data_wifi = 0;
void setup() {
  Serial.begin(115200);
  delay(10);
  G[0] = 0.0;
  G[1] = 0.0;
  G[2] = 0.0;
  
  Serial.begin(9600);
  Wire.begin(2, 14);
  accel.begin(BMA250_range_2g, BMA250_update_time_16ms);//This sets up the BMA250 accelerometer
  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  //    USE_SERIAL.println();
  //    USE_SERIAL.println();
  //    USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//int value = 0;

void loop() {
  //  delay(5000);
  //  ++value;

  //  Serial.print("connecting to ");
  //  Serial.println(host);


  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 3001;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  if (millis() - 20 >= timer_read_from_accel) {
    //Serial.println(timer_read_from_accel);
    timer_read_from_accel = millis();
    accel.read();//This function gets new data from the accelerometer
//Serial.println("\\/RAW DATA \\/");
//Serial.println(String(accel.X) + "," + String(accel.Y) + "," + String(accel.Z));
//Serial.println("/\\RAW END /\\");
//    Serial.print(alpha);
//    Serial.print(" ");
//    Serial.print(G[0]);
//    Serial.print(" ");
//    Serial.print(float(accel.X));
    
    
//    G[0] = alpha * G[0] + (1.0 - alpha) * float(accel.X);
//    G[1] = alpha * G[1] + (1.0 - alpha) * float(accel.Y);
//    G[2] = alpha * G[2] + (1.0 - alpha) * float(accel.Z);
    
    
//    Serial.print(" -> ");
//    Serial.println(G[0]);
    
    //char* row;
    //sprintf(row, "%d,%d,%d", accel.X - G[0], accel.Y - G[1], accel.Z - G[2]);
    //Serial.println(G[0]);
    //Serial.println(G[1]);
    //Serial.println(G[2]);
    row += String(accel.X) + "," + String(accel.Y) + "," + String(accel.Z);
//    row += String(float(accel.X) - G[0]) + "," + String(float(accel.Y) - G[1]) + "," + String(float(accel.Z) - G[2]);
//    Serial.println(row);
  }

//  Serial.println(row);
  // We now create a URI for the request
  String url = "/stream";
  String tsData = String(row);
  //Serial.println(tsData);

  //  Serial.print("Requesting URL: ");
  //  Serial.println(url);


if (millis() - 200 >= timer_send_data_wifi) {
    timer_send_data_wifi = 0;

  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(tsData.length());
  client.print("\r\n\r\n");
  client.print(tsData);
Serial.println(tsData);
  client.print("Connection: close\r\n\r\n");


    row = "";
    // Read all the lines of the reply from server and print them to Serial
    //  while(client.available()){
    //    String line = client.readStringUntil('\r');
    //    Serial.print(line);
    //  }

    //  Serial.println();
    //  Serial.println("closing connection");
  }
}

