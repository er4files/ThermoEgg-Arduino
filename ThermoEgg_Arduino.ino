#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

// Inisialisasi variabel untuk DHT11
#define DHTPIN 5      // GPI05
#define DHTTYPE DHT11

// Buat objek sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Konfigurasi WiFi
const char* ssid = "NAMA WIFI";
const char* password = "PASSWORD WIFI";

// Alamat IP server
const char* server = "192.168.1.1";

// Pin untuk indikator LED
#define pinLED 4 // GPI04

void setup() {
  // Aktifkan Serial Monitor
  Serial.begin(9600);

  // Start sensor DHT11
  dht.begin();

  // Set mode pin LED
  pinMode(pinLED, OUTPUT);

  // Koneksi ke WiFi
  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, password);

  // Uji koneksi WiFi
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(pinLED, LOW);
    delay(500);
  }

  // Jika berhasil terkoneksi
  digitalWrite(pinLED, HIGH);
}

void loop() {
  // Baca nilai suhu dan kelembaban
  float temp = dht.readTemperature();
  int hum = dht.readHumidity();

  // Tampilkan di Serial Monitor
  Serial.println("Suhu: " + String(temp));
  Serial.println("Kelembaban: " + String(hum));

  // Pengiriman data sensor ke URL pertama
  String url1 = "http://" + String(server) + "/thermoegg/public/simpan/" + String(temp) + "/" + String(hum);
  WiFiClient client1;
  HTTPClient http1;
  http1.begin(client1, url1);
  http1.GET();
  http1.end();

  // Pengiriman data sensor ke URL kedua
  String url2 = "http://" + String(server) + "/thermoegg/public/kirimdata/" + String(temp) + "/" + String(hum);
  WiFiClient client2;
  HTTPClient http2;
  http2.begin(client2, url2);
  http2.GET();
  http2.end();

  delay(1000);
}