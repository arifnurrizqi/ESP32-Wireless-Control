/*
  Rui Santos modified by Arif Nur Rizqi
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  github repositories https://github.com/arifnurrizqi/esp-waterflow-monitoring
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define tombol_1  19 // pin of water flow sensor in pin 19
#define tombol_2  18

// Replace with your network credentials
const char* ssid     = "Wokwi-GUEST"; //your wifi ssid
const char* password = ""; //your wifi password

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://tugas.myskill.hydroflow.id/post-data.php"; // "/post-data.php" its endpoint

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

bool PB1 = false;
bool PB2 = false;

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

  pinMode(tombol_1 , INPUT);
  pinMode(tombol_2 , INPUT);

}

void loop() {

  if(digitalRead(tombol_1) == HIGH){
    PB1 =! PB1;
    Serial.print("tombol 1 : ");
    Serial.println(PB1);
    postData(PB1, PB2);
    delay(1000);
  }
  if(digitalRead(tombol_2) == HIGH){
    PB2 =! PB2;
    Serial.print("tombol 2 : ");
    Serial.println(PB2);
    postData(PB1, PB2);
    delay(1000);
  }

}

void postData(bool PB1, bool PB2){
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http; 
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(PB1)
                          + "&value2=" + String(PB2)+ "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
  
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
      
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      if (httpResponseCode == 200){
        Serial.println("Data Terkirim✅");
      }
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Serial.println("Data Gagal Terkirim❌");
    }
     // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}