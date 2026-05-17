#include "mqtt.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

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
    } else {
      Serial.printf("Fehler: %d, erneuter Versuch in 5s\n", mqttClient.state());
      delay(5000);
    }
  }
}

void mqttSetup() {
  wifiConnect();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}

void mqttLoop() {
  if (!mqttClient.connected()) {
    mqttConnect();
  }
  mqttClient.loop();
}

void mqttPublish(const char* topic, const char* payload) {
  mqttClient.publish(topic, payload);
}
