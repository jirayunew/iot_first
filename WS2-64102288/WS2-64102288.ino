#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
float temperature;
float humidity;
DHT dht14(D4,DHT11);

void setup() {
  Serial.begin (9600);
  dht14.begin();
  lcd.begin();
}

void loop() {
  humidity = (dht14.readHumidity());
  temperature = (dht14.readTemperature());
  lcd.setCursor(0, 0);
  lcd.print("Hum: "+String(humidity,1)+" %");     
  lcd.setCursor(0, 1);
  lcd.print("Temp: "+String(temperature,1)+" C");
  delay(1000);
  
}
