#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include "DHT.h"

#define DHTPIN 21
#define DHTTYPE DHT22

const int moistureLevelSensor = 15;

int moistureLevel;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

char blynkApiKey[] = "<blynk_api_key>";

char ssid[] = "<ssid_goes_here>";
char pass[] = "<password_goes_here>";
int analogDry = 400; 
int analogWet = 150;
int percDry = 0;
int percWet = 100;



void sendSensor1()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

}

void sendSensor2()
{
  moistureLevel = constrain(analogRead(moistureLevelSensor),analogWet,analogDry);
  moistureLevel = map(moistureLevel,analogWet,analogDry,percWet,percDry);

  Blynk.virtualWrite(V8, moistureLevel);
}



void setup() {
  Serial.begin(9600);
  Blynk.begin(blynkApiKey, ssid, pass);

  dht.begin();

  timer.setInterval(2000L, sendSensor1);
  timer.setInterval(5000L, sendSensor2);

}


void loop() {
  Blynk.run();
  timer.run();
}
