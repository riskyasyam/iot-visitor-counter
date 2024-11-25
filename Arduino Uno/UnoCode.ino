#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>

#define DHTPIN 7            // Pin data untuk sensor DHT22
#define DHTTYPE DHT22       // Tipe sensor DHT
DHT dht(DHTPIN, DHTTYPE);   // Inisialisasi DHT

#define BUZZER_PIN 6         // Pin untuk buzzer
#define LED_PIN_1 4          // Pin untuk LED 1
#define LED_PIN_2 5          // Pin untuk LED 2

#define ULTRASONIC_PIN_1 3   // Pin signal untuk sensor ultrasonik pertama
#define ULTRASONIC_PIN_2 2   // Pin signal untuk sensor ultrasonik kedua

#define SDA_PIN 10           // Pin SDA (SS) untuk RFID
#define RST_PIN 9            // Pin reset untuk RFID

MFRC522 mfrc522(SDA_PIN, RST_PIN);  // Inisialisasi RFID

String allowedIds[] = {"afda771f", "b488a0b0", "58a86145bc10"}; 
bool isInside[] = {false, false, false};
int totalIds = sizeof(allowedIds) / sizeof(allowedIds[0]);

int pengunjungDiDalam = 0;
int pengunjungMasuk = 0;
int pengunjungKeluar = 0;
bool sensor1Activated = false;
bool sensor2Activated = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  dht.begin();
  Serial.println("Setup selesai. Sistem siap.");

  pinMode(ULTRASONIC_PIN_1, OUTPUT);  
  pinMode(ULTRASONIC_PIN_2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
}

long readUltrasonic(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH);
  long distance = (duration / 29) / 2;
  return distance;
}

void kirimDataPengunjung(float suhu, float kelembapan) {
  // Format data untuk dikirim ke Wemos
  String data = String(pengunjungMasuk) + "," + 
                String(pengunjungKeluar) + "," + 
                String(pengunjungDiDalam) + "," + 
                String(suhu, 2) + "," + 
                String(kelembapan, 2);
  Serial.println("Data dikirim ke Wemos: " + data);
  Serial.println();
}

void loop() {
  long distance1 = readUltrasonic(ULTRASONIC_PIN_1);
  long distance2 = readUltrasonic(ULTRASONIC_PIN_2);

  if (distance1 < 50) {
    sensor1Activated = true;
  }
  if (distance2 < 50) {
    sensor2Activated = true;
  }

  if (sensor1Activated && sensor2Activated) {
    if (distance1 < distance2) {
      pengunjungDiDalam++;
      pengunjungMasuk++;
      Serial.println("Pengunjung masuk.");
    } else {
      pengunjungDiDalam--;
      pengunjungKeluar++;
      Serial.println("Pengunjung keluar.");
    }
    sensor1Activated = false;
    sensor2Activated = false;
  }

  if (distance1 < 50) {
    digitalWrite(LED_PIN_2, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN_2, LOW);
  }

  if (distance2 < 50) {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN_1, LOW);
  }

  if (distance1 >= 50 && distance2 >= 50) {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Baca suhu dan kelembapan dari sensor DHT22
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
  } else {
    kirimDataPengunjung(suhu, kelembapan);
  }

  delay(2000);
}
