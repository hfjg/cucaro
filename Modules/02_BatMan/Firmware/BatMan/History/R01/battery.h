#ifndef __battery_h__
#define __battery_h__

#include <Arduino.h>
#include "ctwi.h"

enum btype {
  BAT_WET_LEAD_ACID,
  BAT_SEALED_LEAD_ACID,
  BAT_NIMH,
  BAT_LIPO,
  BAT_LIFEPO,
  NO_MORE_BTYPES
};

typedef struct {
  char name[7];
  uint16_t cellVoltageMin;
  uint16_t cellVoltageChargeBegin;
  uint16_t cellVoltageMax;
  int8_t tMin, tMax;
}s_batteryData;


const s_batteryData batteryTable[] PROGMEM ={
  {"WLA", 1800, 2300, 2400, -20, 50}, // BAT_WET_LEAD_ACID,
  {"SLA", 1900, 2200, 2300, -20, 50}, // BAT_SEALED_LEAD_ACID,
  {"NiMH", 980, 1020, 1220, 0, 45}, // BAT_NIMH,
  {"LiPo", 3000, 3500, 4200, 10, 45}, // BAT_LIPO
  {"LiFePo", 3200, 3600, 4050, 10, 45} // BAT_LIFEPO
};

uint8_t batterySetup(s_i2cControlRegs *p);

#endif
