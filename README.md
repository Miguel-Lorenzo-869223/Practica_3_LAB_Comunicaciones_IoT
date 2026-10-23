# Practica_3_LAB_Comunicaciones_IoT

# Practical Exercise 3: Wireless Networks, IP Sockets, Web Servers, SenML Interoperability & ThingSpeak Cloud Integration

> **IoT Communications Laboratory**  
> MSc in Electronic Engineering — University of Zaragoza (EINA/UNIZAR)  
> **Author:** Miguel A. Lorenzo  
> **Date:** October 2026  

---

## 📌 Project Overview

This repository contains the complete embedded C++ and Python software implementations for **Practical Exercise 3** of the IoT Communications Laboratory. The main objective is to explore, implement, and analyze both low-level socket-based networking and high-level web service architectures using the **ESP32** microcontroller over Wi-Fi networks.

The lab is divided into eight progressive practical exercises covering network stack initialization, system time synchronization, interactive socket servers, web server UI endpoints, data interoperability standards, and cloud integration:
1. **Wi-Fi Station Setup & Connectivity Diagnostics:** Station mode initialization and HTTP port 80 socket verification.
2. **Atomic Time Synchronization (NTP):** Internal RTC synchronization with POSIX European DST timezone rules.
3. **TCP Socket Timestamp Streaming:** Non-blocking 1-second interval timestamp streaming over raw TCP sockets.
4. **FSM Controlled TCP Communication (ENSEÑAR 1):** Command-driven Finite State Machine over TCP sockets (`start`/`stop`).
5. **Bidirectional TCP Accelerometer Telemetry (ENSEÑAR 2):** ESP32 socket client streaming 3-axis accelerometer telemetry to a host Python TCP server.
6. **Embedded HTTP Web Server (ENSEÑAR 3):** Standalone web server featuring a dynamic HTML UI, clock reset (`/reset`), and NTP re-sync (`/sync`) routes.
7. **SenML Payload Standard (RFC 8428):** Interoperable multi-measurement JSON payload serialization using `ArduinoJson`.
8. **ThingSpeak Cloud Integration:** Real-time cloud telemetry publishing via HTTP REST API calls.

---

## 🛠️ Hardware Setup & Key Peripherals

- **ESP32 Development Board:**
  - **Wi-Fi Transceiver:** Integrated 2.4 GHz IEEE 802.11 b/g/n radio controller operating in Station (`WIFI_STA`) mode.
  - **Internal Clock & RTC:** Hardware Real-Time Clock synchronized via POSIX NTP infrastructure (`time.h`, `sys/time.h`).
  - **Sensors / Signal Generators:** 3-axis accelerometer stream emulator and mathematical sine-wave generator (`math.h`).
- **Diagnostic Tools & Interfaces:**
  - **UART Interface:** Serial communication at **115200 baud** for real-time debugging.
  - **Host Workstation / Mobile Device:** PC running Python 3 runtime, SocketTest utility, or Android mobile device running Simple TCP Socket Tester.

---

## 📱 Required Applications & Software Environment

- **Development IDE:** Arduino IDE (v2.x or later).
- **Core Board Package:**
  - `esp32` by Espressif Systems.
- **Required C++ Libraries:**
  - `WiFi.h` & `WebServer.h` (ESP32 native networking framework).
  - `time.h` & `sys/time.h` (POSIX NTP time infrastructure).
  - `ArduinoJson` (v6.x or v7.x, for SenML RFC 8428 payload serialization).
  - `ThingSpeak` (Official MathWorks ThingSpeak client library).
  - `ESP32Ping` (ICMP network ping diagnostic utility).
- **Host Applications & Software Tools:**
  - **Python 3.x:** Host environment with standard `socket` and `time` modules.
  - **[SocketTest](https://sockettest.sourceforge.net/):** Desktop GUI tool for raw TCP/IP client/server socket testing.
  - **[Simple TCP Socket Tester](https://play.google.com/store/apps/details?id=com.simplesockettester&hl=en_US):** Android application for mobile TCP socket debugging.

---

## 🏗️ Exercises & System Architectures

### 1. Exercise 1.1 — Wi-Fi Network Connection & Google HTTP Check
- **Platform:** ESP32 Dev Module
- **Description:** Configures the Wi-Fi stack in Station Mode (`WIFI_STA`), connects to an access point, prints the DHCP assigned local IP address to Serial Monitor, and opens a raw TCP socket connection to `google.com` on port 80 to verify WAN routing.
- **Key Features:** Automatic network reconnection loop, non-blocking network state checking, and socket status verification via `WiFiClient`.

### 2. Exercise 1.2 — System Clock Synchronization via NTP
- **Platform:** ESP32 Dev Module
- **Description:** Synchronizes the internal system RTC using public NTP servers (`pool.ntp.org`, `time.nist.gov`) and configures European POSIX timezone formatting (`CET-1CEST,M3.5.0,M10.5.0/3`).
- **Key Features:** Automatic Daylight Saving Time (DST) calculation, POSIX timezone string binding via `configTzTime()`, and structured local time formatting (`struct tm`).

### 3. Exercise 1.3 & 1.4 — Basic TCP Socket Client & Timestamp Streaming
- **Platform:** ESP32 Dev Module
- **Description:** Establishes a TCP client connection to a target socket server (SocketTest / Mobile TCP Tester) and streams formatted `YYYY-MM-DD HH:MM:SS` timestamps over the connection every second.
- **Key Features:** Non-blocking timing using `millis()`, active socket verification, and continuous timestamp payload transmission.

### 4. Exercise 1.5 — FSM Controlled TCP Socket Communication (ENSEÑAR 1)
- **Platform:** ESP32 Dev Module
- **Description:** Implements a control-layer Finite State Machine (`STATE_STOPPED`, `STATE_RUNNING`) driven by incoming ASCII commands over an active TCP socket connection.
- **Key Features:** Parses incoming text commands (`start` / `stop`), executes non-blocking state transitions, returns command acknowledgment strings, and conditionally streams timestamp records at 1 Hz.

### 5. Exercise 2 — ESP32 TCP Accelerometer Telemetry & Python Server (ENSEÑAR 2)
- **Platform:** ESP32 Dev Module & Host PC (Python 3)
- **Description:** Establishes a custom TCP socket connection between the ESP32 client and a dedicated host Python TCP server (`main.py`). The ESP32 streams 3-axis accelerometer values (`ACCEL:X,Y,Z`) upon receiving start triggers from the Python host.
- **Key Features:** Handshake protocol synchronization (`start` / `stop`), CSV payload formatting, multithreaded socket reception in Python, and real-time telemetry console output.

### 6. Exercise 3 — Embedded HTTP Web Server with Clock & Reset (ENSEÑAR 3)
- **Platform:** ESP32 Dev Module
- **Description:** Deploys an HTTP web server on port 80 using `<WebServer.h>`. Serves an interactive HTML interface displaying real-time NTP clock values with custom operational endpoints.
- **Key Features:** HTML UI with automatic meta-refresh (2 seconds), `/reset` route zeroing system time to `00:00:00` via `settimeofday()`, and `/sync` route forcing clock synchronization against NTP servers.

### 7. Exercise 4 — SenML Standard JSON Data Generator
- **Platform:** ESP32 Dev Module
- **Description:** Generates standard-compliant Sensor Measurement Lists (SenML, RFC 8428) JSON telemetry payloads using the `ArduinoJson` library every 10 seconds.
- **Key Features:** Structured array serialization incorporating Base Name (`bn`), Base Time (`bt`), Base Unit (`bu`: `"Cel"`), Name (`n`), Value (`v`), and Time offset (`t`).

### 8. Exercise 5 — ThingSpeak Cloud Telemetry Integration
- **Platform:** ESP32 Dev Module
- **Description:** Integrates the ESP32 with the **ThingSpeak** IoT Cloud platform using HTTP REST API updates via the official `ThingSpeak` C++ library.
- **Key Features:** Generates a continuous mathematical sine-wave signal using `math.h` and publishes real-time updates to Field 1 of a cloud channel every 16 seconds.

---

## 📂 Project Structure

```text
.
├── LB_IOT_P3_1_WiFi_Ping/
│   └── LB_IOT_P3_1_WiFi_Ping.ino          # Exercise 1.1: Wi-Fi Station & Google Port 80 Check
├── LB_IOT_P3_2_NTP_Clock/
│   └── LB_IOT_P3_2_NTP_Clock.ino          # Exercise 1.2: System Clock Synchronization via NTP
├── LB_IOT_P3_3_Socket_Time/
│   └── LB_IOT_P3_3_Socket_Time.ino        # Exercises 1.3-1.4: TCP Client Timestamp Streamer
├── LB_IOT_P3_4_Socket_FSM/
│   └── LB_IOT_P3_4_Socket_FSM.ino         # Exercise 1.5: FSM Controlled TCP Communication (ENSEÑAR 1)
├── LB_IOT_P3_5_TCP_Accel_Stream/
│   ├── LB_IOT_P3_5_TCP_Accel_Stream.ino   # Exercise 2: ESP32 TCP Accelerometer Client (ENSEÑAR 2)
│   └── main.py                            # Exercise 2: Python TCP Host Server Script
├── LB_IOT_P3_6_WebServer_Clock/
│   └── LB_IOT_P3_6_WebServer_Clock.ino    # Exercise 3: Embedded HTTP Web Server & Clock UI (ENSEÑAR 3)
├── LB_IOT_P3_7_SenML_JSON/
│   └── LB_IOT_P3_7_SenML_JSON.ino         # Exercise 4: SenML RFC 8428 JSON Payload Generator
├── LB_IOT_P3_8_ThingSpeak_Cloud/
│   └── LB_IOT_P3_8_ThingSpeak_Cloud.ino   # Exercise 5: ThingSpeak Cloud REST Integration
└── README.md                              # Repository documentation
```

---

## 📚 Reference Material & Documentation

- **Lab Manual Support Links:**
  - [ESP32 NTP Server & Date/Time Tutorial (LastMinuteEngineers)](https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/)
  - [SocketTest Desktop Application Download](https://sockettest.sourceforge.net/)
  - [Simple TCP Socket Tester (Android Google Play)](https://play.google.com/store/apps/details?id=com.simplesockettester&hl=en_US)
  - [Python Simple TCP/IP Server and Client Guide](https://thatgirlcoder.com/2021/09/01/how-to-create-a-simple-python-tcp-ip-server-and-client/)
  - [ESP32 Web Server Tutorial (RandomNerdTutorials)](https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/)

- **Official Specifications & API References:**
  - [RFC 8428 — Sensor Measurement Lists (SenML) Specification](https://datatracker.ietf.org/doc/html/rfc8428)
  - [ArduinoJson Official Library Documentation](https://arduinojson.org/)
  - [ThingSpeak REST API Documentation](https://es.mathworks.com/help/thingspeak/)

---

## 🚀 How to Build & Run

### Running TCP Socket & FSM Exercises (Exercises 1.1 – 1.5)
1. Open **SocketTest** on your PC (or **Simple TCP Socket Tester** on Android) and start a TCP Server listening on port `487`.
2. Configure Wi-Fi credentials (`ssid`, `password`) and target `serverIP` in `LB_IOT_P3_4_Socket_FSM.ino`.
3. Select **ESP32 Dev Module** in **Arduino IDE**, upload the sketch, and open the Serial Monitor at **115200 baud**.
4. In SocketTest, send the string `start` to trigger 1-second timestamp streaming, and `stop` to halt execution.

### Running Python Accelerometer Telemetry (Exercise 2)
1. Determine your computer's local IP address (`ipconfig` on Windows or `ifconfig` on Linux/Mac).
2. Launch the host Python TCP server in your terminal:
   ```bash
   python main.py
   ```
3. Enter your computer's IP address in `serverIP` inside `LB_IOT_P3_5_TCP_Accel_Stream.ino` and upload to the ESP32.
4. Observe connection acceptance in the Python terminal, automatic transmission of the `start` command, and live rendering of incoming `ACCEL:X,Y,Z` telemetry stream.

### Running Embedded Web Server (Exercise 3)
1. Upload `LB_IOT_P3_6_WebServer_Clock.ino` to the ESP32.
2. Read the assigned local IP address printed on the Serial Monitor (e.g., `192.168.1.150`).
3. Open a web browser on any device connected to the same network and navigate to `http://<ESP32_IP>`.
4. Test the **RST** button to zero system time to `00:00:00`, and click **Sync NTP** to trigger immediate time re-synchronization.

### Running SenML Generator & ThingSpeak Cloud (Exercises 4 & 5)
1. **SenML Generator:** Upload `LB_IOT_P3_7_SenML_JSON.ino` and monitor standard RFC 8428 formatted JSON strings outputting to the Serial Monitor every 10 seconds.
2. **ThingSpeak Integration:** Create a free channel on [ThingSpeak.com](https://thingspeak.com/), insert your `myChannelNumber` and `myWriteAPIKey` into `LB_IOT_P3_8_ThingSpeak_Cloud.ino`, and upload. Inspect live sine-wave plots updating every 16 seconds on your cloud dashboard.
