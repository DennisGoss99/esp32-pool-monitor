#pragma once
#include <stdint.h>

struct SensorConfig {
  uint8_t  samples;   // moving average samples (1-50)
  uint32_t interval;  // send interval in seconds (1-3600)
};

void sensorConfigLoad();
void sensorConfigSave();
SensorConfig& sensorConfigGet();
