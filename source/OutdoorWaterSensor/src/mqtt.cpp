#include "mqtt.h"
#include "config.h"
#include "sensorconfig.h"
#include "temperature.h"
#include "light.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

static WiFiClient   wifiClient;
static PubSubClient mqttClient(wifiClient);

static void publishConfig() {
  SensorConfig& cfg = sensorConfigGet();
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", cfg.samples);
  mqttClient.publish(MQTT_TOPIC_CONFIG_SAMPLES_STATE, buf, true);
  snprintf(buf, sizeof(buf), "%d", cfg.interval);
  mqttClient.publish(MQTT_TOPIC_CONFIG_INTERVAL_STATE, buf, true);
}

static void onMessage(char* topic, byte* payload, unsigned int length) {
  char msg[16];
  length = length < sizeof(msg) - 1 ? length : sizeof(msg) - 1;
  memcpy(msg, payload, length);
  msg[length] = '\0';

  SensorConfig& cfg = sensorConfigGet();

  if (strcmp(topic, MQTT_TOPIC_CONFIG_SAMPLES) == 0) {
    uint8_t val = (uint8_t)atoi(msg);
    if (val >= 1 && val <= 50) {
      cfg.samples = val;
      temperatureSetSamples(val);
      lightSetSamples(val);
      sensorConfigSave();
      publishConfig();
      Serial.printf("Samples gesetzt: %d\n", val);
    }
  } else if (strcmp(topic, MQTT_TOPIC_CONFIG_INTERVAL) == 0) {
    uint32_t val = (uint32_t)atoi(msg);
    if (val >= 1 && val <= 3600) {
      cfg.interval = val;
      sensorConfigSave();
      publishConfig();
      Serial.printf("Interval gesetzt: %ds\n", val);
    }
  }
}

static void wifiConnect() {
  Serial.printf("Verbinde mit WLAN: %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWLAN verbunden. IP: %s\n", WiFi.localIP().toString().c_str());
}

static void mqttConnect() {
  while (!mqttClient.connected()) {
    Serial.println("Verbinde mit MQTT...");
    if (mqttClient.connect("ESP32_Outdoor", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT verbunden!");
      mqttClient.subscribe(MQTT_TOPIC_CONFIG_SAMPLES);
      mqttClient.subscribe(MQTT_TOPIC_CONFIG_INTERVAL);
      publishConfig();
    } else {
      Serial.printf("Fehler: %d, erneuter Versuch in 5s\n", mqttClient.state());
      delay(5000);
    }
  }
}

void mqttSetup() {
  wifiConnect();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(onMessage);
}

void mqttLoop() {
  if (!mqttClient.connected()) mqttConnect();
  mqttClient.loop();
}

void mqttPublish(const char* topic, const char* payload) {
  mqttClient.publish(topic, payload);
}
