#include <Arduino.h>
#include "temperature.h"
#include "light.h"
#include "mqtt.h"
#include "config.h"
#include "sensorconfig.h"

void setup() {
  Serial.begin(115200);
  sensorConfigLoad();
  temperatureSetup(4);
  temperatureSetSamples(sensorConfigGet().samples);
  lightSetup();
  lightSetSamples(sensorConfigGet().samples);
  mqttSetup();
  Serial.println("Sensoren bereit");
}

void loop() {
  mqttLoop();

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

  delay(sensorConfigGet().interval * 1000);
}
