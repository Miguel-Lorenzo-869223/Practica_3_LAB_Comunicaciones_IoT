// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 08/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------
#include <WiFi.h>

// Configuración de la red Wi-Fi
const char* ssid     = "Redmi Note 13 Pro+ 5G";
const char* password = "ccw6xiuq7esytfj";

// Configuración del Servidor TCP (Pon la IP de TU PC)
const char* serverIP = "10.44.84.148"; 
const int serverPort = 5000; // Usar puerto libre > 1024

// Máquina de estados
enum SystemState {
  STATE_STOPPED,
  STATE_RUNNING
};

SystemState currentState = STATE_STOPPED;
WiFiClient client;
unsigned long lastSendTime = 0;

// Declaración de funciones
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
      // En reposo: esperando comando 'start'
      break;
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectada.");
  Serial.print("IP local de la ESP32: ");
  Serial.println(WiFi.localIP());
}

void manageConnection() {
  if (!client.connected()) {
    Serial.println("Conectando al servidor TCP...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Conectado al servidor con éxito.");
    } else {
      Serial.println("Error de conexión. Reintentando...");
      delay(2000);
    }
  }
}

void handleSocketCommands() {
  while (client.available() > 0) {
    String incomingMsg = client.readStringUntil('\n');
    incomingMsg.trim();

    if (incomingMsg.equalsIgnoreCase("start")) {
      currentState = STATE_RUNNING;
      Serial.println("Estado FSM -> STATE_RUNNING");
      client.println(">> OK: Transmision iniciada");
    } 
    else if (incomingMsg.equalsIgnoreCase("stop")) {
      currentState = STATE_STOPPED;
      Serial.println("Estado FSM -> STATE_STOPPED");
      client.println(">> OK: Transmision detenida");
    }
  }
}

void sendSimulatedAccelData() {
  if (millis() - lastSendTime >= 1000) { // Envía datos cada 1 segundo
    lastSendTime = millis();

    // Generar datos aleatorios simulados (-1.00 a 1.00 m/s²)
    float ax = random(-100, 101) / 100.0;
    float ay = random(-100, 101) / 100.0;
    float az = random(-100, 101) / 100.0;

    // Enviar por el socket TCP en formato CSV: ACCEL:X,Y,Z
    String payload = "ACCEL:" + String(ax, 2) + "," + String(ay, 2) + "," + String(az, 2);
    client.println(payload);

    // Mostrar por monitor serie
    Serial.print("Enviado por TCP: ");
    Serial.println(payload);
  }
}