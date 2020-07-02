
#include <Arduino.h>
#include "ctwi.h"
#include "battery.h"
#include "errors.h"


uint8_t batterySetup(s_i2cControlRegs *cr){
  s_batteryData batteryCellData;
  uint8_t n;

  n = cr->numCells;
  memcpy_P(
    &batteryCellData,
    &(batteryTable[cr->batteryType]),
    sizeof(batteryCellData)
  );
  cr->voltageMin = n * batteryCellData.cellVoltageMin;
  cr->voltageChargeBegin = n * batteryCellData.cellVoltageChargeBegin;
  cr->voltageMax = n * batteryCellData.cellVoltageMax;
  cr->tMin = batteryCellData.tMin;
  cr->tMax = batteryCellData.tMax;
  cr->vChargerMin = cr->voltageMax + 3000;
  cr->vChargerMax = 25000;
  cr->iBatteryCharge = cr->capacityMilliAmpHour;

  return E_NO_ERROR;
}
