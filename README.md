# Visitor Monitoring System (Arduino Uno & Wemos)

Proyek ini adalah sistem monitoring pengunjung yang terdiri dari dua perangkat: **Arduino Uno** dan **Wemos (ESP8266)**. Arduino Uno bertugas mengumpulkan data sensor dan mengirimkannya ke Wemos, yang kemudian mengirimkan data tersebut ke server API.

### Fitur
- **Arduino Uno** mengumpulkan data suhu, kelembapan, dan jumlah pengunjung.
- **Wemos (ESP8266)** menerima data dari Arduino, memformat data, dan mengirimnya ke server API dalam format JSON.

---

## Persyaratan

- **Arduino Uno**
- **Wemos (ESP8266)**
- **Modul DHT11 atau DHT22** (untuk suhu dan kelembapan)
- **Modul RFID atau sensor gerak** (untuk mendeteksi jumlah pengunjung, opsional)
- **WiFi** yang terhubung dengan internet

---

## Langkah-Langkah Penggunaan

### 1. Menyiapkan Koneksi WiFi
Sebelum meng-upload kode ke Wemos, Anda perlu mengubah SSID dan password WiFi agar sesuai dengan jaringan lokal Anda.

- Buka file `WemosCode.ino` pada Arduino IDE.
- Temukan bagian berikut di kode:

    ```cpp
    const char* ssid = "ZTE_2.4G_36kdi9";  // Ganti dengan SSID WiFi Anda
    const char* password = "12345678";     // Ganti dengan password WiFi Anda
    ```

- Ganti `ssid` dan `password` dengan kredensial WiFi Anda.

### 2. Mengubah Nama Server API
Pada bagian kode Wemos, terdapat URL server API yang digunakan untuk mengirimkan data. Anda perlu menyesuaikan server API ini dengan API yang Anda gunakan. Berikut adalah bagian yang perlu diubah:

```cpp
const String serverName = "http://192.168.1.6:3000/api/post-visitors";  // Ganti dengan URL API Anda
