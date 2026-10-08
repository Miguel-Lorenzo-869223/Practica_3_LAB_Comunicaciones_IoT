// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 08/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <WiFi.h>
#include <time.h>
#include <math.h>

// WiFi Configuration
const char* ssid     = "Redmi Note 13 Pro+ 5G";
const char* password = "ccw6xiuq7esytfj";

// Socket Server Configuration
const char* serverIP = "10.44.84.148"; // IP de tu ordenador
const int serverPort = 5000;           // Puerto TCP

// Finite State Machine (FSM) States
enum SystemState {
  STATE_STOPPED,
  STATE_RUNNING
};

SystemState currentState = STATE_STOPPED;

WiFiClient client;
unsigned long lastSendTime = 0;
float angle = 0.0; // Variable para simular variación de aceleración

// Function Declarations
void connectToWiFi();
void manageConnection();
void handleSocketCommands();
void sendSimulatedAccelData();

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  manageConnection();
}

void loop() {
  manageConnection();
  handleSocketCommands();

  switch (currentState) {
    case STATE_RUNNING:
      sendSimulatedAccelData();
      break;

    case STATE_STOPPED:
      // Waiting for 'start' command
      break;
  }
}

// Connects to WiFi AP
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected.");
}

// Manages TCP Connection
void manageConnection() {
  if (!client.connected()) {
    Serial.println("Connecting to TCP Server...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Connected to TCP Server successfully.");
    } else {
      Serial.println("Connection failed. Retrying...");
      delay(2000);
    }
  }
}

// Processes incoming control commands ('start' / 'stop')
void handleSocketCommands() {
  while (client.available() > 0) {
    String incomingMsg = client.readStringUntil('\n');
    incomingMsg.trim();

    if (incomingMsg.equalsIgnoreCase("start")) {
      currentState = STATE_RUNNING;
      Serial.println("FSM State -> STATE_RUNNING");
      client.println(">> OK: Streaming STARTED");
    } 
    else if (incomingMsg.equalsIgnoreCase("stop")) {
      currentState = STATE_STOPPED;
      Serial.println("FSM State -> STATE_STOPPED");
      client.println(">> OK: Streaming STOPPED");
    }
  }
}

// Generates and transmits simulated accelerometer data (X,Y,Z)
void sendSimulatedAccelData() {
  if (millis() - lastSendTime >= 200) { // Envía cada 200 ms
    lastSendTime = millis();

    // Generación de datos simulados (en m/s²)
    angle += 0.1;
    float simulated_x = sin(angle) * 9.81;
    float simulated_y = cos(angle) * 4.90;
    float simulated_z = 9.81 + (sin(angle * 2) * 0.5);

    // Format: ACCEL:X,Y,Z
    String payload = "ACCEL:" + String(simulated_x, 2) + "," + 
                                String(simulated_y, 2) + "," + 
                                String(simulated_z, 2);

    client.println(payload);
    Serial.print("Sent: ");
    Serial.println(payload);
  }
}