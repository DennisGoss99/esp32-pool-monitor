#include "light.h"
#include "movingaverage.h"
#include <BH1750.h>
#include <Wire.h>

static BH1750 lightMeter;
static MovingAverage<float, 10> avg;

void lightSetup() {
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

void lightSetSamples(uint8_t samples) {
  avg.setSize(samples);
}

float lightGetLux() {
  avg.add(lightMeter.readLightLevel());
  return avg.get();
}
