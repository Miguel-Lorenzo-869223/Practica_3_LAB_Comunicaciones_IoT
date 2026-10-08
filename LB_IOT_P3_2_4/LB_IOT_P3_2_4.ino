// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 08/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <WiFi.h>
#include <time.h>

// Configuración de red WiFi
const char* ssid     = "ssid";
const char* password = "pswd";

// Configuración del Servidor Socket de destino
const char* serverIP = "TU IP"; // IP de la máquina donde corre SocketTest / Servidor
const int serverPort = 8981;           // Puerto donde escucha el servidor

// Configuración del servidor NTP para obtener la hora local
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // Offset UTC en segundos (ej. UTC+1 = 3600)
const int   daylightOffset_sec = 3600; // Horario de verano (si aplica)

WiFiClient client;
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(115200);

  // 1. Conexión a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado.");

  // 2. Configurar la hora desde NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  // Esperar a que se sincronice la hora
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Esperando sincronización NTP...");
    delay(1000);
  }
  Serial.println("Hora sincronizada correctamente.");

  // 3. Conectar al Servidor Socket
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Conectado al servidor socket correctamente.");
  } else {
    Serial.println("Error de conexión con el servidor.");
  }
}

void loop() {
  // Verificar que haya pasado 1 segundo (1000 ms) sin bloquear con delay
  if (millis() - lastSendTime >= 1000) {
    lastSendTime = millis();

    // Reconectar si la conexión se interrumpió
    if (!client.connected()) {
      Serial.println("Reconectando al servidor...");
      client.connect(serverIP, serverPort);
    }

    if (client.connected()) {
      // Obtener la hora local formateada
      struct tm timeinfo;
      if (getLocalTime(&timeinfo)) {
        char timeStringBuff[50];
        // Formato HH:MM:SS (puedes ajustar el formato si el test te pide algo específico)
        strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", &timeinfo); 

        // Enviar el mensaje por el socket
        client.println(timeStringBuff);
        Serial.print("Enviado: ");
        Serial.println(timeStringBuff);
      }
    }
  }
}