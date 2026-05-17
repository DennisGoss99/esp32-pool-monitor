#pragma once
#include <stdint.h>

void temperatureSetup(uint8_t pin);
void temperatureSetSamples(uint8_t samples);
float temperatureGetC();
