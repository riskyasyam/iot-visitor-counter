#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi Credentials
const char* ssid = "ZTE_2.4G_36kdi9";
const char* password = "12345678";
const String serverName = "http://192.168.1.6:3000/api/post-visitors";

// WiFi and Time Setup
WiFiClient wifiClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // UTC +7 (WIB)

// Data Variables
String lastDate = "";
int pengunjungDiDalam = 0;
int pengunjungMasuk = 0;
int pengunjungKeluar = 0;
float suhu = 0.0;
float kelembapan = 0.0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Terhubung ke WiFi!");
  timeClient.begin(); // Start NTP client
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();

    // Convert time to date format (YYYY-MM-DD)
    time_t rawTime = timeClient.getEpochTime();
    struct tm* timeInfo = gmtime(&rawTime);
    char formattedDate[11]; // YYYY-MM-DD + null terminator
    strftime(formattedDate, sizeof(formattedDate), "%Y-%m-%d", timeInfo);
    String currentDate = String(formattedDate);

    // Check if data is available from Arduino
    if (Serial.available()) {
      String data = Serial.readStringUntil('\n'); // Read data until newline
      data.trim(); // Remove any trailing or leading whitespace
      Serial.println("Data diterima dari Uno: " + data);

      // Remove prefix (e.g., "kirim ke Wemos:")
      int startIndex = data.indexOf(":");
      if (startIndex != -1) {
        data = data.substring(startIndex + 1); // Remove everything before ':'
        data.trim(); // Clean up leading/trailing whitespace
      }

      Serial.println("Data setelah prefix dihapus: " + data);

      // Parse the data
      int separator1 = data.indexOf(',');
      int separator2 = data.indexOf(',', separator1 + 1);
      int separator3 = data.indexOf(',', separator2 + 1);
      int separator4 = data.indexOf(',', separator3 + 1);

      if (separator1 != -1 && separator2 != -1 && separator3 != -1 && separator4 != -1) {

        // Debug and parse pengunjungMasuk
        String debugFirstData = data.substring(0, separator1);
        Serial.println("Substring pertama (pengunjungMasuk): '" + debugFirstData + "' (dengan tanda petik)");
        debugFirstData.trim(); // Remove whitespace
        pengunjungMasuk = debugFirstData.toInt();
        Serial.println("Hasil pengunjungMasuk: " + String(pengunjungMasuk));

        pengunjungKeluar = data.substring(separator1 + 1, separator2).toInt();
        pengunjungDiDalam = data.substring(separator2 + 1, separator3).toInt();
        suhu = data.substring(separator3 + 1, separator4).toFloat();
        kelembapan = data.substring(separator4 + 1).toFloat();

        Serial.println("Parsed data:");
        Serial.println("Pengunjung Masuk: " + String(pengunjungMasuk));
        Serial.println("Pengunjung Keluar: " + String(pengunjungKeluar));
        Serial.println("Pengunjung Di Dalam: " + String(pengunjungDiDalam));
        Serial.println("Suhu: " + String(suhu));
        Serial.println("Kelembapan: " + String(kelembapan));

        // Reset data if the date changes
        if (currentDate != lastDate) {
          Serial.println("Tanggal berubah. Reset data.");
          pengunjungMasuk = 0;
          pengunjungKeluar = 0;
          pengunjungDiDalam = 0;
          lastDate = currentDate;
        }

        // Create JSON payload
        String jsonData = "{";
        jsonData += "\"nama_wisata\": \"BoroBoro\",";
        jsonData += "\"tanggal\": \"" + currentDate + "\",";
        jsonData += "\"pengunjung_masuk\": " + String(pengunjungMasuk) + ",";
        jsonData += "\"pengunjung_keluar\": " + String(pengunjungKeluar) + ",";
        jsonData += "\"pengunjung_di_dalam\": " + String(pengunjungDiDalam) + ",";
        jsonData += "\"suhu\": " + String(suhu) + ",";
        jsonData += "\"kelembapan\": " + String(kelembapan);
        jsonData += "}";

        // Send data to the server
        HTTPClient http;
        http.begin(wifiClient, serverName);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode > 0) {
          Serial.println("Data berhasil dikirim!");
          Serial.println("Payload: " + jsonData);
        } else {
          Serial.println("Gagal mengirim data.");
        }
        http.end();
      } else {
        Serial.println("Data tidak valid, parsing gagal.");
      }
    }
  }

  delay(2000); // Delay to avoid spamming
}
