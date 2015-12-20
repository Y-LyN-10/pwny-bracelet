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

String row =""; 

const char* ssid     = "HackBulgaria";
const char* password = "";

const char* host = "10.0.203.231";
const char* streamId   = "....................";
const char* privateKey = "....................";

float G[3] = {0.0, 0.0, 0.0};
float alpha = 0.9;

long timer_read_from_accel = 0;
long timer_send_data_wifi = 0;
void setup() {
  Serial.begin(115200);
  delay(10);


  Serial.begin(9600);
  Wire.begin(2, 14);
  accel.begin(BMA250_range_2g, BMA250_update_time_4ms);//This sets up the BMA250 accelerometer
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
Serial.println(timer_read_from_accel);
timer_read_from_accel = millis();
  accel.read();//This function gets new data from the accelerometer

  G[0] = alpha * G[0] + (1 - alpha) * accel.X;
  G[1] = alpha * G[1] + (1 - alpha) * accel.Y;
  G[2] = alpha * G[2] + (1 - alpha) * accel.Z;

  //char* row;
  //sprintf(row, "%d,%d,%d", accel.X - G[0], accel.Y - G[1], accel.Z - G[2]);

  row += String(int(accel.X - G[0])) + "," + String(int(accel.Y - G[1])) + "," + String(int(accel.Z - G[2]));
  //  Serial.println(row);
} 

Serial.println(row); 
  // We now create a URI for the request
  String url = "/stream";
  String tsData = String(row);
  //Serial.println(tsData);

  //  Serial.print("Requesting URL: ");
  //  Serial.println(url);



  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(tsData.length());
  client.print("\r\n\r\n");
  client.print(tsData);
  client.print("Connection: close\r\n\r\n");

if (millis() - 200 >= timer_send_data_wifi) {
 timer_send_data_wifi = 0;
 
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

