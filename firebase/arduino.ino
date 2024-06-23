//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!
//UNFINISED!!!!!!!!!!!

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

// Replace these with your WiFi credentials and Firebase host/key
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define FIREBASE_HOST "your_firebase_database_url"
#define FIREBASE_AUTH "your_firebase_database_secret"

// Initialize the TinyGPS++ library
TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3); // RX, TX

// Set up Firebase
FirebaseData firebaseData;

void setup() {
  // Start the serial communication for debugging
  Serial.begin(9600);
  // Start the serial communication with the GPS module
  gpsSerial.begin(9600);
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    
    if (gps.location.isUpdated()) {
      sendDataToFirebase();
    }
  }
}

void sendDataToFirebase() {
  if (Firebase.ready()) {
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
    String speed = String(gps.speed.kmph());
    String direction = String(gps.course.deg());

    // Update the values in the Firebase database
    Firebase.setString(firebaseData, "/carTelemetry/latitude", latitude);
    Firebase.setString(firebaseData, "/carTelemetry/longitude", longitude);
    Firebase.setString(firebaseData, "/carTelemetry/speed", speed);
    Firebase.setString(firebaseData, "/carTelemetry/direction", direction);

    Serial.println("Data sent to Firebase:");
    Serial.println("Latitude: " + latitude);
    Serial.println("Longitude: " + longitude);
    Serial.println("Speed: " + speed);
    Serial.println("Direction: " + direction);
  }
}
