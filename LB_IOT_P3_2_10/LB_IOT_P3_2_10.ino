// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 09/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <WiFi.h>
#include <math.h>       // Library for sin() trigonometric function
#include "ThingSpeak.h" // Official ThingSpeak library

// --- NETWORK AND THINGSPEAK CONFIGURATION ---
const char* ssid     = "ssid";
const char* password = "pswd";

// Replace with your actual ThingSpeak credentials
unsigned long myChannelNumber = xxxxxxx;            // Your Channel ID
const char* myWriteAPIKey     = "Your Write API Key"; // Your Write API Key

WiFiClient client;
unsigned long lastPostTime = 0;

// --- FUNCTION DECLARATIONS ---
void connectToWiFi();
void initThingSpeak();
float generateSineWave();
void sendDataToThingSpeak();

void setup() {
  Serial.begin(115200);

  // Initialize network and cloud client
  connectToWiFi();
  initThingSpeak();
}

void loop() {
  // Ensure Wi-Fi connection remains active
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // Execute periodic non-blocking data transmission
  sendDataToThingSpeak();
}

// --- FUNCTION DEFINITIONS ---

// Connects the ESP32 to the configured Wi-Fi network
void connectToWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected successfully.");
  Serial.print("ESP32 Local IP: ");
  Serial.println(WiFi.localIP());
}

// Initializes communication with the ThingSpeak server
void initThingSpeak() {
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak client initialized.");
}

// Calculates and returns a smooth sine wave value based on elapsed time
float generateSineWave() {
  float timeSec = millis() / 1000.0;
  // Amplitude = 10.0 | Frequency factor = 0.05 for smooth plotting at 16s intervals
  return 10.0 * sin(timeSec * 0.05);
}

// Formats and sends data to ThingSpeak every 16 seconds (non-blocking)
void sendDataToThingSpeak() {
  // ThingSpeak free tier requires a 15s+ interval between updates
  if (millis() - lastPostTime >= 16000) {
    lastPostTime = millis();

    // 1. Calculate sine wave value
    float sineWave = generateSineWave();

    // 2. Set value for Field 1
    ThingSpeak.setField(1, sineWave);

    // 3. Write fields to ThingSpeak channel
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if (httpCode == 200) {
      Serial.print("Data sent to ThingSpeak successfully. Sine Value: ");
      Serial.println(sineWave);
    } else {
      Serial.print("Error sending to ThingSpeak. HTTP Code: ");
      Serial.println(httpCode);
    }
  }
}