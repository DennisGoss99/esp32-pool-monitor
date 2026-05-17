#pragma once

void mqttSetup();
void mqttLoop();
void mqttPublish(const char* topic, const char* payload);
