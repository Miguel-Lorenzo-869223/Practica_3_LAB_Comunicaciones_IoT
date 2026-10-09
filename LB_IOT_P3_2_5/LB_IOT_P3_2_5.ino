
// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 08/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

/*
struct tm {
  int tm_sec;   // Segundos (0-60)
  int tm_min;   // Minutos (0-59)
  int tm_hour;  // Horas (0-23)
  int tm_mday;  // Día del mes (1-31)
  int tm_mon;   // Mes (0-11)
  int tm_year;  // Año desde 1900
  int tm_wday;  // Día de la semana (0-6)
  int tm_yday;  // Día del año (0-365)
  int tm_isdst; // Horario de verano (sí/no)
};
*/


#include <WiFi.h>
#include <time.h>

// WiFi Configuration
const char* ssid     = "SSID";
const char* password = "pwsd";

// Socket Server Configuration
const char* serverIP = "aaa.bbb.ccc.ddd"; // Target PC IP
const int serverPort = 487 ;            // Target Server Port

// NTP Server Configuration
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // UTC+1
const int   daylightOffset_sec = 3600; // Daylight saving time

// Finite State Machine (FSM) States
enum SystemState {
  STATE_STOPPED,
  STATE_RUNNING
};

SystemState currentState = STATE_STOPPED;

WiFiClient client;
unsigned long lastSendTime = 0;

// --- FUNCTION DECLARATIONS ---
void connectToWiFi();
void syncNTP();
void manageConnection();
void handleSocketCommands();
void sendCurrentTime();

void setup() {
  Serial.begin(115200);

  // Initialize network and time synchronization
  connectToWiFi();
  syncNTP();
  manageConnection();
}

void loop() {
  // Ensure TCP connection stays alive
  manageConnection();

  // Process incoming commands from the server
  handleSocketCommands();

  // Execute state machine logic
  switch (currentState) {
    case STATE_RUNNING:
      sendCurrentTime();
      break;

    case STATE_STOPPED:
      // Idle state: waiting for 'start' command
      break;
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
}

// Synchronizes ESP32 internal clock with NTP server
void syncNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for NTP synchronization...");
    delay(1000);
  }
  Serial.println("Time synchronized successfully.");
}

// Manages TCP connection and attempts reconnecting if disconnected
void manageConnection() {
  if (!client.connected()) {
    Serial.println("Connecting to Socket Server...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Connected to Socket Server successfully.");
      Serial.println("Send 'start' or 'stop' commands from SocketTest.");
    } else {
      Serial.println("Connection to Socket Server failed.");
      delay(2000);
    }
  }
}

// Reads incoming socket data and triggers state transitions
void handleSocketCommands() {
  static String rxBuffer = ""; // Maintains buffer state between function calls

  while (client.available() > 0) {
    char c = client.read(); // Read byte by byte instantly without waiting

    if (c == '\n') { // Line finished, process command
      rxBuffer.trim(); // Remove whitespaces and \r

      if (rxBuffer.equalsIgnoreCase("start")) {
        currentState = STATE_RUNNING;
        Serial.println("FSM State -> STATE_RUNNING");
        client.println(">> OK: Time streaming STARTED");
      } 
      else if (rxBuffer.equalsIgnoreCase("stop")) {
        currentState = STATE_STOPPED;
        Serial.println("FSM State -> STATE_STOPPED");
        client.println(">> OK: Time streaming STOPPED");
      }

      rxBuffer = ""; // Reset buffer for the next message
    } 
    else if (c != '\r') {
      rxBuffer += c; // Append character to buffer
    }
  }
}

// Formats local time and sends it through the TCP socket every second
void sendCurrentTime() {
  if (millis() - lastSendTime >= 1000) {
    lastSendTime = millis();

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeStringBuff[50];
      strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", &timeinfo);

      client.println(timeStringBuff);
      Serial.print("Sent: ");
      Serial.println(timeStringBuff);
    }
  }
}