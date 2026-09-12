// Librerías para sincronización NTP en ESP32
#include <WiFi.h>
#include <time.h>

// Credenciales de la red Wi-Fi
const char* ssid = "Redmi Note 13 Pro+ 5G";
const char* password = "ccw6xiuq7esytfj";

// Servidores NTP públicos
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";

// Cadena POSIX para España peninsular / Europa Central (CET/CEST)
// CET-1CEST: UTC+1 en invierno, UTC+2 en verano
// M3.5.0: Cambio a verano el último domingo de marzo
// M10.5.0/3: Cambio a invierno el último domingo de octubre a las 3:00
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3";

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡WiFi Conectado!");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());
}

void syncTime() {
  // Configura el cliente NTP interno de la ESP32
  configTime(0, 0, ntpServer1, ntpServer2);

  // Aplica la zona horaria europea/española
  setenv("TZ", tzInfo, 1);
  tzset();

  Serial.println("Sincronizando hora con servidor NTP...");
  
  // Espera a recibir un timestamp válido (posterior al 1 de enero de 2020)
  time_t now = 0;
  while (time(&now) < 1577836800) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Hora sincronizada correctamente!");
}

// Función para imprimir la fecha en formato DD/MM/AAAA HH:MM:SS
void printDateTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 2000)) {
    Serial.println("Error al obtener la hora");
    return;
  }

  char formattedTime[80];
  // %d = Día (01-31), %m = Mes (01-12), %Y = Año (4 dígitos)
  // %H = Hora (00-23), %M = Minuto (00-59), %S = Segundo (00-59)
  strftime(formattedTime, sizeof(formattedTime), "%d/%m/%Y %H:%M:%S", &timeinfo);
  
  Serial.print("Fecha y Hora actual: ");
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
  delay(1000); // Muestra la hora cada segundo
}