#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial esp8266(2, 3); // RX, TX

#define OLED_ADDR   0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

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

  // Initialize OLED display (optional)
  initOLED();
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
      // Send data to Netlify server via ESP8266
      sendSensorData(ax, ay, az, latitude, longitude);
    }
  }

  // Display data on OLED display (optional)
  displayDataOnOLED(ax, ay, az);

  delay(1000); // Adjust delay based on your requirements
}

void sendSensorData(float ax, float ay, float az, float lat, float lon) {
  String postData = "ax=" + String(ax) + "&ay=" + String(ay) + "&az=" + String(az);
  postData += "&lat=" + String(lat, 6) + "&lon=" + String(lon, 6);

  String postRequest = "POST / HTTP/1.1\r\n";
  postRequest += "Host: " + String(host) + "\r\n";
  postRequest += "Content-Type: application/x-www-form-urlencoded\r\n";
  postRequest += "Content-Length: " + String(postData.length()) + "\r\n\r\n";
  postRequest += postData;

  sendDataToESP8266("AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(port));
  delay(1000);
  sendDataToESP8266("AT+CIPSEND=" + String(postRequest.length()));
  delay(500);
  sendDataToESP8266(postRequest);
  delay(500);
}

void sendDataToESP8266(String command) {
  esp8266.println(command);
  delay(100);
  while (esp8266.available()) {
    String response = esp8266.readString();
    Serial.println(response);
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

void initOLED() {
  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0xAE); // Display off
  Wire.write(0xD5); // Set display clock divide ratio/oscillator frequency
  Wire.write(0x80); // Set divide ratio
  Wire.write(0xA8); // Set multiplex ratio
  Wire.write(0x3F); // Set to 64
  Wire.write(0xD3); // Set display offset
  Wire.write(0x00); // Set offset
  Wire.write(0x40); // Set start line
  Wire.write(0x8D); // Charge pump setting
  Wire.write(0x14); // Enable charge pump
  Wire.write(0x20); // Set memory mode
  Wire.write(0x00); // Horizontal addressing mode
  Wire.write(0xA1); // Set segment re-map
  Wire.write(0xC8); // Set COM output scan direction
  Wire.write(0xDA); // Set COM pins hardware configuration
  Wire.write(0x12); // Alternative COM pin configuration
  Wire.write(0x81); // Set contrast control
  Wire.write(0xCF); // Set contrast
  Wire.write(0xD9); // Set pre-charge period
  Wire.write(0xF1); // Set pre-charge period
  Wire.write(0xDB); // Set VCOMH
  Wire.write(0x40); // Set VCOMH
  Wire.write(0xA4); // Set entire display on/off
  Wire.write(0xA6); // Set normal display
  Wire.write(0xAF); // Display on
  Wire.endTransmission();
}

void displayDataOnOLED(float ax, float ay, float az) {
  // Display data on OLED (example)
  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x00); // Command mode
  Wire.write(0x10); // Column start address
  Wire.write(0x00); // Lower nibble of column start address
  Wire.write(0x7F); // Upper nibble of column start address
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x40); // Data mode
  for (int i = 0; i < 64 * 128 / 8; i++) {
    Wire.write(0x00); // Clear display
  }
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x40); // Data mode
  for (int y = 0; y < 8; y++) {
    Wire.write(0x00); // Clear display
  }
  Wire.endTransmission();
}
