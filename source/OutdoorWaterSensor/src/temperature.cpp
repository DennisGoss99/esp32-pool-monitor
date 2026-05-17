#include "temperature.h"
#include "movingaverage.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

static OneWire oneWire(0);
static DallasTemperature sensors(&oneWire);
static MovingAverage<float, 10> avg;

void temperatureSetup(uint8_t pin) {
  oneWire.begin(pin);
  sensors.begin();
}

void temperatureSetSamples(uint8_t samples) {
  avg.setSize(samples);
}

float temperatureGetC() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  if (temp == DEVICE_DISCONNECTED_C) return NAN;
  avg.add(temp);
  return avg.get();
}
