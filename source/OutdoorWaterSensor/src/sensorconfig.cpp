#include "sensorconfig.h"
#include <Preferences.h>

static SensorConfig config = { 10, 10 };
static Preferences prefs;

void sensorConfigLoad() {
  prefs.begin("pool", true);
  config.samples  = prefs.getUChar("samples", 10);
  config.interval = prefs.getUInt("interval", 10);
  prefs.end();
}

void sensorConfigSave() {
  prefs.begin("pool", false);
  prefs.putUChar("samples",  config.samples);
  prefs.putUInt("interval",  config.interval);
  prefs.end();
}

SensorConfig& sensorConfigGet() {
  return config;
}
