#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define led1 4 // pin of boot button for reset total volume
#define led2 2 // pin of battery voltage in pin 36

int status_tombol1, status_tombol2;

// Replace with your network credentials
const char* ssid     = "Wokwi-GUEST"; //your wifi ssid
const char* password = ""; //your wifi password

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://tugas.myskill.hydroflow.id/get-data.php"; // "/get-data.php" its endpoint
String apiKeyValue = "tPmAT5Ab3j7F9";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

 // Inisialisasi pin LED sebagai output
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
}

void loop() {

  // Buat URL dengan menyertakan API key sebagai parameter
  String url = String(serverName) + "?api_key=" + apiKeyValue;
  // Lakukan permintaan HTTP GET ke API
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode == 200) {
    Serial.println("Berhasil Mendapatkan Data✅");
    String payload = http.getString();
    // Parsing JSON
    StaticJsonDocument<200> jsonDocument;
    deserializeJson(jsonDocument, payload);

    Serial.println(payload);

    // Mengambil nilai status_tombol1
    status_tombol1 = jsonDocument["data"]["data1"];   
    if(status_tombol1 >= 1){
      digitalWrite(led1, HIGH);
    } else {
      digitalWrite(led1, LOW);
    }
    // Mengambil nilai status_tombol2
    status_tombol2 = jsonDocument["data"]["data2"];   
    if(status_tombol2 >= 1){
      digitalWrite(led2, HIGH);
    } else {
      digitalWrite(led2, LOW);
    }
      
  } else {
    Serial.print("HTTP GET failed, error code: ");
    Serial.println(httpResponseCode);
  }
  http.end(); // Tutup koneksi HTTP

}
