//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
//UNFINISHED!!!!!!!!!!!!!!
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TimeLib.h>   // Time library
#include <WiFi.h>      // WiFi library

SoftwareSerial esp8266(2, 3); // RX, TX

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
TinyGPSPlus gps;

const char* ssid = "YourCarWiFiSSID";
const char* password = "YourCarWiFiPassword";
const char* host = "oksrctelem.netlify.app"; // Replace with your Netlify site hostname
const int port = 80;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  accel.begin();
  esp8266.begin(9600);

  // Connect to WiFi
  connectWiFi();
}

void loop() {
  // Read accelerometer data
  sensors_event_t event;
  accel.getEvent(&event);
  float ax = event.acceleration.x;
  float ay = event.acceleration.y;
  float az = event.acceleration.z;

  // Read GPS data
  while (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      
      // Get current time
      String currentTime = getCurrentTime();

      // Send data to ESP8266
      sendSensorData(ax, ay, az, latitude, longitude, currentTime);
    }
  }

  delay(1000); // Adjust delay based on your requirements
}

String getCurrentTime() {
  // Get current time
  time_t now = time(nullptr);
  struct tm * timeinfo;
  timeinfo = localtime(&now);
  char timeStr[20];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(timeStr);
}

void sendSensorData(float ax, float ay, float az, float lat, float lon, String timeStr) {
  String url = "/data?ax=" + String(ax) +
               "&ay=" + String(ay) +
               "&az=" + String(az) +
               "&latitude=" + String(lat, 6) +
               "&longitude=" + String(lon, 6) +
               "&time=" + timeStr;

  if (client.connect(host, port)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10); // Give the server some time to process

    Serial.println("Sent data to server:");
    Serial.println(url);

    // Read and display response
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
  } else {
    Serial.println("Failed to connect to server");
  }
}

void connectWiFi() {
  esp8266.println("AT+RST");
  delay(1000);
  esp8266.println("AT+CWMODE=1");
  delay(1000);
  esp8266.println("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"");
  delay(3000);
}