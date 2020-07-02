/**
 \mainpage

 # BatMan
 BatMan ist the Battery Manager Module for CuCaRo. It is closely coupled to
 the Central Electronics Module (Raspberry Pi 4) via TWI-Bus.

 ## Hardware
 The BatMan hardware consists of
 ### Microcontroller
    <a href="https://www.arduino.cc/en/pmwiki.php?n=Main/ArduinoBoardProMini">
    Arduino Pro Mini</a> (5V, 16MHz)
 ### Level Shifter
    Level shifter module for the connection with the RaspberryPi4 I2C
 ### Current sensor module
    <a href="https://www.allegromicro.com/~/media/files/datasheets/acs712-datasheet.ashx">ACS712-20</a> (+/-20A, sensitivity 100mV/A, offset
    voltage ist 2500mV)
 ### Analog-Digital-Converter
    16-Bit-Analog-digital-converter module
    <a href="http://www.ti.com/lit/ds/symlink/ads1115.pdf">
    ADS1115</a> with integrated precision reference.
    This is necessary to achieve sufficient resolution and precision
    of voltage measurement when using Lithium-based batteries and also for
    sufficient precision of the current measurement wit th
    ACS712-20 module.
  ### DC-DC-Converters
  - a fixed-output DC-DC-Buck-Converter V_Bat -> 5V @ 10A for servos
  - a fixed-output DC-DC-Buck-Converter V_Bat -> 5V @ 3A for the CEM
  - a variable-output DC-DC-Converter for V_Charge -> V_Bat

  ### Power on/off/reset button
    One button for on/off if pressed for a short time, reboot if pressed for a
      long time


  ## Software
  The software takes care of following tasks:
  - Take periodic samples of battery and charger voltages, current and
    temperature and update I2C registers accordingly, force shutdown after
    communication timeout of battery breakdown.
  - Control charging (if charger attached)
  - Receive and execute commands in I2C command register
  - Check button and execute command accordingly

Configuration is read from and written to EEPROM. If EEPROM routines find a
checksum error, EEPROM and I2C registers are set to defaults.

  \todo Implement small battery for controlled shutdown if battery was
      removed accidentally.
**/

#include <Arduino.h>
#include <EEPROM.h>

#include "hardware.h"
#include "ctwi.h"
#include "adc.h"
#include "keys.h"
#include "charge.h"
#include "battery.h"
#include "helpers.h"
#include "errors.h"

#define DEBUG


#ifdef DEBUG
#define dprint(x) Serial.print(x)
#else
#define dprint(x) ;
#endif

s_i2cReadRegs i2cReadRegs;
s_i2cControlRegs i2cControlRegs;


/**
 \brief Call all initialization routines
**/
void setup() {

  hardwareInit();
  ctwiInit(&i2cReadRegs, &i2cControlRegs);
  adcInit(1);

#ifdef DEBUG
  Serial.begin(115200);

  Serial.println(i2cControlRegs.voltageMin);
  Serial.println(i2cControlRegs.voltageChargeBegin);
  Serial.println(i2cControlRegs.voltageMax);
  Serial.println(i2cControlRegs.tMin);
  Serial.println(i2cControlRegs.tMax);
  Serial.println(i2cControlRegs.vChargerMin);
  Serial.println(i2cControlRegs.vChargerMax);
  Serial.println(i2cControlRegs.iBatteryCharge);
#endif
}


/**
 \brief Save state to EEPROM, then shutdown
**/
void shutdown(void){
  EEPROM.put(0, i2cControlRegs);
  EEPROM.put(sizeof(s_i2cControlRegs),
      checksum((uint8_t *)(&i2cControlRegs), sizeof(s_i2cControlRegs)) );

  // switch off myself
  digitalWrite(PIN_POWER_HOLD, 0);
}

/**
* \brief Main loop
**/
void loop() {
uint32_t lastSample, shutdownTriggerTime;

  lastSample = millis();

  //sampleADC();
  getVoltageMillivolts(&(i2cReadRegs.vCharger), ADS_V_CHARGER, 68, 10);
  getVoltageMillivolts(&(i2cReadRegs.vBattery), ADS_V_BATTERY, 33, 10);
  getCurrentACS712(&(i2cReadRegs.iBattery), ADS_I_BATTERY, 20);
  getVoltageMillivolts((uint16_t*)(&(i2cReadRegs.tBattery)), ADS_T_BATTERY, 1, 1);

  dprint("\nUpdated i2cReadRegs");

  // set status bits
  i2cReadRegs.state.flag.batteryLow =
    (i2cReadRegs.vBattery <= i2cControlRegs.voltageChargeBegin);

  // i2cRegs are updated in background, check for twi commands
  if (i2cControlRegs.command.shutdown){
    i2cReadRegs.state.flag.shutdownAck = 1;
    i2cControlRegs.command.shutdown = 0; //reset command
    shutdownTriggerTime = lastSample;
  }


  // control charger
  int16_t ichg = charge(&i2cReadRegs, &i2cControlRegs);
  dprint("\nCharge PWM : "); dprint(ichg);

  switch(checkKey(PIN_BUTTON)){
    case KEY_SHORT:
      i2cReadRegs.state.flag.shutdownRequest = 1;
      i2cReadRegs.state.flag.rebootRequest = 0;
      break;
    case KEY_LONG:
      i2cReadRegs.state.flag.shutdownRequest = 0;
      i2cReadRegs.state.flag.rebootRequest = 1;
      break;
    default:
      i2cReadRegs.state.flag.shutdownRequest = 0;
      i2cReadRegs.state.flag.rebootRequest = 0;
      break;
  }

  if ( (millis() - shutdownTriggerTime) >= i2cControlRegs.shutdownTimeMillis)
    shutdown();

  // idle for rest of sampling time
  while ( (millis()-lastSample) < i2cControlRegs.sampleTimeMillis)
    ;



}
