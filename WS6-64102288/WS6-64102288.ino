#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

const char* ssid = "New Jey Sey_2.4G";
const char* password = "0994061612";
const char* mqtt_server = "192.168.2.41";

const int ledPin = D6;

WiFiClient espClient;
PubSubClient client(espClient);

  float Temp;
  float Hum;
  bool Led;

DHT dht(D4, DHT11);

ESP8266WebServer server(80);


unsigned long lastLoopTime = 0;
const unsigned long loopInterval = 5000;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "led") {
    if (message == "on") {
      digitalWrite(ledPin, HIGH);
    } else if (message == "off") {
      digitalWrite(ledPin, LOW);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Temp = dht.readTemperature();
  Hum = dht.readHumidity();
  IPAddress localIP = WiFi.localIP();

  StaticJsonDocument<200> doc;
  doc["temperature"] = Temp;
  doc["humidity"] = Hum;
  doc["ip"] = localIP.toString(); 

  String jsonStr;
  serializeJson(doc, jsonStr);

  client.publish("dht11", jsonStr.c_str());

  server.handleClient(); 
  delay(5000);
}


