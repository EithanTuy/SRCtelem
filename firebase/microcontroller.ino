//////////////////////////////////Unfinished
#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace these with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Your Firebase Project API key and URL
#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "your-database-secret-or-auth-key"

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to Wi-Fi");
  
  // Set Firebase host and auth key
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float speed = 20.5; // Example speed
  String location = "29.76,-95.36"; // Example location coordinates
  
  // Push speed and location data to Firebase
  if (Firebase.setFloat(firebaseData, "/car/speed", speed)) {
    Serial.println("Speed data sent successfully");
  } else {
    Serial.println("Failed to send speed data");
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.setString(firebaseData, "/car/location", location)) {
    Serial.println("Location data sent successfully");
  } else {
    Serial.println("Failed to send location data");
    Serial.println(firebaseData.errorReason());
  }
  
  delay(5000); // Update every 5 seconds
}
