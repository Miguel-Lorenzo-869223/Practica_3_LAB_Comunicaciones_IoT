// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 02/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <WiFi.h>

const char* ssid = "SSID";
const char* password = "pswd";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nConectando a WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n¡WiFi conectado!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Comprobar conectividad TCP a Google
  Serial.println("Comprobando conectividad con google.com...");
  WiFiClient client;
  
  if (client.connect("google.com", 80)) {
    Serial.println("¡Conexión exitosa a Google! (HTTP 80 OK)");
    client.stop();
  } else {
    Serial.println("Error: No se pudo conectar a Google.");
  }
}

void loop() {
  // Sin código en el bucle principal
}