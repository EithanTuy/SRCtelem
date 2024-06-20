//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
//UNFINISHED!!!!!
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial esp8266(2, 3); // RX, TX

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
TinyGPSPlus gps;

const char* ssid = "YourCarWiFiSSID";
const char* password = "YourCarWiFiPassword";
const char* host = "oksrctelem.netlify.app"; // Replace with your Netlify site hostname
const int port = 80;

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
      // Send data to ESP8266
      sendSensorData(ax, ay, az, latitude, longitude);
    }
  }

  delay(1000); // Adjust delay based on your requirements
}

void sendSensorData(float ax, float ay, float az, float lat, float lon) {
  String responseData = String(ax) + "," + String(ay) + "," + String(az) + "," + String(lat, 6) + "," + String(lon, 6);
  Serial.println(responseData); // Send the formatted data over serial to ESP8266
}

void connectWiFi() {
  esp8266.println("AT+RST");
  delay(1000);
  esp8266.println("AT+CWMODE=1");
  delay(1000);
  esp8266.println("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"");
  delay(3000);
}
