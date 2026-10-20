// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 20/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

/*
 * -----------------------------------------------------------------
 * SenML JSON Structure (RFC 8428 Standard):
 * -----------------------------------------------------------------
 * [
 *   {
 *     "bn": "urn:dev:esp32:",  // Base Name: Unique device identifier (URN)
 *     "bt": 1792523720,        // Base Time: Unix Epoch timestamp in seconds
 *     "bu": "Cel",             // Base Unit: Standardized unit ("Cel" = °C)
 *     "n" : "temperature",     // Name: Measured resource/sensor name
 *     "v" : 23.4,              // Value: Numeric measurement reading
 *     "t" : 0                  // Time: Relative offset from Base Time
 *   }
 * ]
 * -----------------------------------------------------------------
 */

#include <WiFi.h>
#include <time.h>
#include <ArduinoJson.h> // Library for JSON creation (ArduinoJson)

// WiFi Configuration
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// NTP Configuration
const char* ntpServer          = "pool.ntp.org";
const long  gmtOffset_sec      = 3600; // UTC+1
const int   daylightOffset_sec = 3600; // Daylight saving time

// Timer Variable for 10-second interval
unsigned long lastSenMLSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds in milliseconds

// --- FUNCTION DECLARATIONS ---
void connectToWiFi();
void syncNTP();
float generateFakeTemperature();
String createSenMLJSON();

void setup() {
  // Initialize serial monitor for debugging
  Serial.begin(115200);

  // Connect to WiFi and synchronize NTP clock for accurate timestamps
  connectToWiFi();
  syncNTP();

  Serial.println("\n--- Starting SenML JSON Generator (Every 10s) ---\n");
}

void loop() {
  // Non-blocking timer: executes every 10 seconds
  if (millis() - lastSenMLSendTime >= sendInterval) {
    lastSenMLSendTime = millis();

    // Generate SenML RFC 8428 compliant JSON string
    String senmlPayload = createSenMLJSON();

    // Print generated SenML JSON payload to Serial Monitor
    Serial.println("Generated SenML JSON:");
    Serial.println(senmlPayload);
    Serial.println("----------------------------------------");
  }
}

// --- FUNCTION DEFINITIONS ---

// Connects the ESP32 to the configured WiFi network
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Synchronizes internal system clock with NTP server
void syncNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  int retries = 0;
  while (!getLocalTime(&timeinfo) && retries < 10) {
    delay(500);
    retries++;
  }
  if (retries < 10) {
    Serial.println(">> Time successfully synchronized with NTP.");
  } else {
    Serial.println(">> Warning: NTP synchronization failed.");
  }
}

// Simulates a realistic temperature reading between 20.0 °C and 28.0 °C
float generateFakeTemperature() {
  return 20.0 + (random(0, 80) / 10.0);
}

// Creates a SenML RFC 8428 JSON payload using ArduinoJson
String createSenMLJSON() {
  // Get current Epoch timestamp in seconds
  time_t now;
  time(&now);

  // Measure fake temperature value
  float currentTemp = generateFakeTemperature();

  // Create JSON document (StaticJsonDocument works seamlessly on ArduinoJson v6 and v7)
  StaticJsonDocument<256> doc;

  // RFC 8428 standard requires the root element to be a JSON Array
  JsonArray senmlArray = doc.to<JsonArray>();

  // Add SenML record object inside the array
  JsonObject record = senmlArray.createNestedObject();
  record["bn"] = "urn:dev:esp32:"; // Base Name (URN Device Identifier)
  record["bt"] = (long)now;        // Base Time (Epoch timestamp in seconds)
  record["bu"] = "Cel";            // Base Unit (RFC 8428 standard unit for Celsius)
  record["n"]  = "temperature";    // Record Name
  record["v"]  = currentTemp;      // Measured Value
  record["t"]  = 0;                // Relative Time offset from Base Time

  // Serialize JSON to string
  String jsonOutput;
  serializeJson(doc, jsonOutput);

  return jsonOutput;
}