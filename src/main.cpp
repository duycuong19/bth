#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE    DHT11 
#define DHTPIN 4  
DHT_Unified dht(DHTPIN,DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("error reading tem!"));
  } else
  {
    Serial.println(F("tem: "));
    Serial.println(event.temperature);
    Serial.println("C");
  }
  delay(1000);
  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity))
  {
    Serial.println(F("humnidity: "));
  } else 
  {
    Serial.println(F("humidity: "));
    Serial.println(event.relative_humidity);
    Serial.println(F("%"));
  }
  delay(1000);
}

