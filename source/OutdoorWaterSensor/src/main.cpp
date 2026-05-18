#include <Arduino.h>
#include "temperature.h"
#include "light.h"
#include "mqtt.h"
#include "config.h"

#ifndef PUBLISH_INTERVAL_MS
#define PUBLISH_INTERVAL_MS 30000
#endif

void setup() {
  Serial.begin(115200);
  temperatureSetup(4);
  lightSetup();
  mqttSetup();
  Serial.println("Sensoren bereit");
}

static unsigned long lastPublish = 0;

void loop() {
  mqttLoop();

  unsigned long now = millis();
  if (now - lastPublish < PUBLISH_INTERVAL_MS) return;
  lastPublish = now;

  float tempC = temperatureGetC();
  float lux   = lightGetLux();

  if (isnan(tempC)) {
    Serial.println("Temperatursensor nicht gefunden!");
  } else {
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", tempC);
    mqttPublish(MQTT_TOPIC_TEMPERATURE, buf);
    Serial.printf("Temperatur: %.2f C\n", tempC);
  }

  char buf[16];
  snprintf(buf, sizeof(buf), "%.1f", lux);
  mqttPublish(MQTT_TOPIC_LIGHT, buf);
  Serial.printf("Helligkeit: %.1f lux\n", lux);
}
