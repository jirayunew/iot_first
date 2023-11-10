#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

const char* ssid = "NewJerSey_";
const char* password = "12345678";

unsigned long chanelID = 2337060;
const char* writeKey = "LTSD38RRYU9XWA1S";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

float temperature;
float humidity;
DHT dht14(D4,DHT11);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  dht14.begin();

}

void loop() {
  if((millis()-lastTime) > timerDelay){
    if(WiFi.status()!= WL_CONNECTED){
      Serial.print("Atteming to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid,password);  
        delay(5000);
      }
      Serial.println("/nConnected.");
    }
    temperature = (dht14.readTemperature());
    humidity = (dht14.readHumidity());
    ThingSpeak.setField(1,temperature);
    ThingSpeak.setField(2,humidity);
    int x = ThingSpeak.writeFields(chanelID, writeKey);
    if(x ==200){
      Serial.println("Channel1 update successful");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code"+ String(x));
    }
    lastTime = millis();
    
    
  }
}
