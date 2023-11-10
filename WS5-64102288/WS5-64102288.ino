#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "NewJerSey_";
const char* password = "12345678";

WiFiClient client;
HTTPClient http;
DHT dht(D4, DHT11); 

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  dht.begin(); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  static unsigned long lastTime = 0;
  unsigned long timerDelay = 15000;
  if ((millis() - lastTime) > timerDelay) {
    float humidity = dht.readHumidity(); 
    float temperature = dht.readTemperature(); 

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor");
    } else {
      Serial.print("Humidity: ");
      Serial.println(humidity);
      Serial.print("Temperature: ");
      Serial.println(temperature);

      // Create JSON object
      StaticJsonDocument<200> jsonDocument;
      jsonDocument["humidity"] = humidity;
      jsonDocument["temperature"] = temperature;
      jsonDocument["timestamp"] = millis();

      // Serialize JSON data to String
      String jsonData;
      serializeJson(jsonDocument, jsonData);

      // Send HTTP POST request
      http.begin(client, "http://172.20.10.3:80/ws5");
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(jsonData);

      if (httpResponseCode > 0) {
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        String payload = http.getString();
        Serial.println("Returned payload:");
        Serial.println(payload);
      } else {
        Serial.println("Error on sending POST: " + String(httpResponseCode));
      }
      http.end();
    }
    lastTime = millis();
  }
}