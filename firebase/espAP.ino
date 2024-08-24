#include <WiFi.h>

// Set your Wi-Fi credentials
const char* ssid = "ESP32-AP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  
  // Set ESP32 as an access point
  WiFi.softAP(telemAP, 12345678);

  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // The Access Point will run indefinitely
}
