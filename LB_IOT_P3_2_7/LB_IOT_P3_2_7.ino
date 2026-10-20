// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 20/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <sys/time.h>

// WiFi Configuration
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// NTP Server Configuration
const long gmtOffset_sec      = 3600; // UTC+1
const int  daylightOffset_sec = 3600; // Daylight saving time

// Web Server instance on port 80
WebServer server(80);

// --- FUNCTION DECLARATIONS ---
void connectToWiFi();
void syncNTP();
void resetClockToZero();
String getFormattedTime();
void handleRoot();
void handleReset();
void handleSync();

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize WiFi connection and synchronize internal clock with NTP
  connectToWiFi();
  syncNTP();

  // Configure Web Server HTTP routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/reset", HTTP_GET, handleReset);
  server.on("/sync", HTTP_GET, handleSync);

  // Start HTTP Web Server
  server.begin();
  Serial.println("Web server started successfully.");
}

void loop() {
  // Listen and handle incoming HTTP client requests
  server.handleClient();
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

// Synchronizes ESP32 internal clock with NTP server
void syncNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
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

// Resets internal clock hours, minutes, and seconds to 00:00:00
void resetClockToZero() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    timeinfo.tm_hour = 0;
    timeinfo.tm_min  = 0;
    timeinfo.tm_sec  = 0;

    time_t t_reset = mktime(&timeinfo);
    struct timeval tv = { .tv_sec = t_reset, .tv_usec = 0 };
    settimeofday(&tv, NULL);
    Serial.println(">> Internal clock reset to 00:00:00.");
  }
}

// Formats local system time to string (YYYY-MM-DD HH:MM:SS)
String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "NTP Time Unavailable";
  }
  char timeBuff[50];
  strftime(timeBuff, sizeof(timeBuff), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeBuff);
}

// Handles root GET request '/': Serves HTML with auto-refresh every 1 second
void handleRoot() {
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='1'>"; // Auto-refresh page every 1s
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Web Server</title>";
  
  // Internal CSS styles
  html += "<style>";
  html += "body { font-family: Helvetica, Arial, sans-serif; text-align: center; margin-top: 40px; }";
  html += "h1 { color: #0F3376; font-size: 2.2rem; }";
  html += "p { font-size: 1.5rem; }";
  html += ".button { display: inline-block; background-color: #008CBA; border: none; border-radius: 4px; color: white; padding: 14px 35px; font-size: 24px; text-decoration: none; margin: 5px; cursor: pointer; }";
  html += ".button2 { background-color: #4CAF50; }";
  html += "</style></head><body>";

  html += "<h1>ESP32 Web Server</h1>";
  html += "<p>Date & Time: <strong>" + getFormattedTime() + "</strong></p>";
  
  // RST button -> Triggers /reset route
  html += "<p><a href='/reset'><button class='button'>RST</button></a></p>";
  
  // Sync NTP button -> Triggers /sync route to reload real NTP time
  html += "<p><a href='/sync'><button class='button button2'>Sync NTP</button></a></p>";
  
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Handles GET request '/reset': Resets clock to 00:00:00 and redirects to root
void handleReset() {
  resetClockToZero();
  server.sendHeader("Location", "/");
  server.send(303); // HTTP redirect
}

// Handles GET request '/sync': Re-synchronizes with NTP server and redirects to root
void handleSync() {
  syncNTP();
  server.sendHeader("Location", "/");
  server.send(303); // HTTP redirect
}