/** ctwi.h
* cucaro twi definitions
*
**/
#ifndef __ctwi_h__
#define __ctwi_h__
#include <Arduino.h>

#define CUCARO_SLAVE_ADDRESS 0x26

typedef struct {
  union{
    struct {
      char batteryOK : 1;
      char batteryLow : 1;
      char chargerConnected : 1;
      char charging : 1;
      char shutdownRequest : 1;
      char shutdownAck : 1;
      char rebootRequest : 1;
      char reserved : 1;
    } flag;
    uint8_t complete;
  } state;
  uint16_t vBattery;
  uint16_t vCharger;
  int16_t iBattery;
  uint8_t tBattery;
}s_i2cReadRegs;

typedef struct {
  struct {
    char shutdown : 1;
  }command;
  uint8_t  batteryType;
  uint8_t  numCells;
  uint16_t capacityMilliAmpHour;
  uint32_t shutdownTimeMillis;
  uint32_t sampleTimeMillis;

  uint16_t voltageMin;
  uint16_t voltageChargeBegin;
  uint16_t voltageMax;
  uint16_t vChargerMin, vChargerMax;
  int16_t iBatteryCharge;

  int8_t tMin, tMax;

}s_i2cControlRegs;


void ctwiInit(s_i2cReadRegs *readRegs, s_i2cControlRegs *ctlRegs);
#endif
