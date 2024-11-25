# Visitor Monitoring System (Arduino Uno & Wemos)

This project is a visitor monitoring system that consists of two devices: **Arduino Uno** and **Wemos (ESP8266)**. The Arduino Uno is responsible for collecting sensor data and sending it to the Wemos, which then sends the data to the API server.

### Features
- **Arduino Uno** collects data on temperature, humidity, and the number of visitors.
- **Wemos (ESP8266)** receives data from Arduino, formats the data, and sends it to the API server in JSON format.

---

## Requirements

- **Arduino Uno**
- **Wemos (ESP8266)**
- **DHT11 or DHT22 Module** (for temperature and humidity)
- **RFID Module or Motion Sensor** (for detecting the number of visitors, optional)
- **WiFi** connected to the internet

---

## How to Use

### 1. Set Up WiFi Connection
Before uploading the code to the Wemos, you need to change the SSID and WiFi password to match your local network.

- Open the `WemosCode.ino` file in the Arduino IDE.
- Locate the following section in the code:

    ```cpp
    const char* ssid = "ZTE_2.4G_36kdi9";  // Replace with your WiFi SSID
    const char* password = "12345678";     // Replace with your WiFi password
    ```

- Replace `ssid` and `password` with your WiFi credentials.

### 2. Change the API Server URL
In the Wemos code, there is a URL for the API server used to send the data. You need to adjust this to the API you are using. Here is the part that needs to be changed:

```cpp
const String serverName = "http://192.168.1.6:3000/api/post-visitors";  // Replace with your API URL
