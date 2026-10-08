// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 02/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <WiFi.h>

// WiFi credentials
const char* ssid = "SSID";
const char* password = "PSWD";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until the connection is established
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Check TCP connectivity to Google
  Serial.println("Checking connectivity to google.com...");
  WiFiClient client;
  
  if (client.connect("google.com", 80)) {
    Serial.println("Successful connection to Google! (HTTP 80 OK)");
    client.stop();
  } else {
    Serial.println("Error: Could not connect to Google.");
  }
}

void loop() {
  // Main loop is intentionally left blank
}