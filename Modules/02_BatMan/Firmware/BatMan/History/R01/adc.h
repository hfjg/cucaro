#ifndef __adc_h__
#define __adc_h__

#include <Arduino.h>

void adcInit(uint8_t gain);

uint8_t getVoltageMillivolts(
  uint16_t *result,
  uint8_t channel,
  uint8_t rp,
  uint8_t rn
);

uint8_t getCurrentACS712(
  int16_t *result,
  uint8_t channel,
  uint8_t iMax
);

#endif
