// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 08/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

// Libraries for NTP synchronization on ESP32
#include <WiFi.h>
#include <time.h>

// Wi-Fi network credentials
const char* ssid = "SSID";
const char* password = "pswd";

// Public NTP servers
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";

// POSIX timezone string for Peninsular Spain / Central Europe (CET/CEST)
// CET-1CEST: UTC+1 in winter, UTC+2 in summer
// M3.5.0: Daylight saving time starts on the last Sunday of March
// M10.5.0/3: Standard time starts on the last Sunday of October at 3:00 AM
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3";

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("Assigned IP: ");
  Serial.println(WiFi.localIP());
}

void syncTime() {
  // Configure the internal ESP32 NTP client
  configTime(0, 0, ntpServer1, ntpServer2);

  // Apply the European/Spanish timezone configuration
  setenv("TZ", tzInfo, 1);
  tzset();

  Serial.println("Synchronizing time with NTP server...");
  
  // Wait to receive a valid timestamp (after January 1, 2020)
  time_t now = 0;
  while (time(&now) < 1577836800) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTime synchronized successfully!");
}

// Function to print the date and time in DD/MM/YYYY HH:MM:SS format
void printDateTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 2000)) {
    Serial.println("Error obtaining time");
    return;
  }

  char formattedTime[80];
  // %d = Day (01-31), %m = Month (01-12), %Y = Year (4 digits)
  // %H = Hour (00-23), %M = Minute (00-59), %S = Second (00-59)
  strftime(formattedTime, sizeof(formattedTime), "%d/%m/%Y %H:%M:%S", &timeinfo);
  
  Serial.print("Current Date and Time: ");
  Serial.println(formattedTime);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectWiFi();
  syncTime();
}

void loop() {
  printDateTime();
  delay(1000); // Display the time every second
}