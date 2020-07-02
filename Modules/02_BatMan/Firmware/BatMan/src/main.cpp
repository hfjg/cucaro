<<<<<<< HEAD
#include <Arduino.h>

#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 adc;

#include <SPI.h>
#include "mcp_can.h"


#include "chargeController.h"

enum {E_NO_ERROR, E_CAN_INIT_ERROR, E_WRONG_PARAMETER};
enum {ADC_VBAT_CHANNEL, ADC_VCHG_CHANNEL, ADC_IBAT_CHANNEL, ADC_TBAT_CHANNEL};
enum {POLARITY_TRUE, POLARITY_INVERTED};

#define spiCSPin 10
MCP_CAN CAN(spiCSPin);
#define MAX_PDO_PERIOD 10000

#define CHARGE_ENABLE_PIN 4
#define CHARGE_ENABLE_INVERTED 0

// Cave! Pro Mini PWM only on 3, 5, 6, 9, 10, 11
#define CHARGE_PWM_PIN 5
#define CHARGE_PWM_INVERTED 1

#define PIN_BUTTON 6

#define PIN_POWER_HOLD 7
#define POLARITY_PIN_POWER_HOLD 1

#define NODE_ID 0x11
#define LOOP_TIME 500

struct {
  union{
    struct {
      char chargerPresent : 1;
    }flag;
    uint8_t statusByte;
  } status;
  int8_t tBatteryCelsius;
  uint16_t vBatteryMillivolt;
  uint16_t iBatteryMilliamp;
  uint16_t vChargerMillivolt;
}processData;


struct{
    uint16_t vBatteryMax;
    uint16_t vBatteryMin;
    uint16_t vBatteryChargeBegin;
    uint16_t iChargeMax;
}sdo;

typedef struct{
  uint8_t rp, rn;
} s_adcCfg;

struct{
  s_adcCfg adcChannelConfig[4]={{1,1},{2,2},{3,3}, {4,4}};
}serviceData;

boolean dataChange;



uint8_t initCAN(uint8_t canBaud){
  if (CAN.begin(CAN_500KBPS) != CAN_OK)
    return E_CAN_INIT_ERROR;
  else
    return E_NO_ERROR;
}

void initADC(adsGain_t  gain){
  adc.begin();
  adc.setGain(gain); // 1x gain   +/- 4.069V  1 bit = 0.125mV
}

uint8_t getADCmillivolts(uint16_t *result, uint8_t channel){
  if (channel > 3)
    return E_WRONG_PARAMETER;
  else {
    *result = adc.readADC_SingleEnded(channel)/adc.getGain()/8;
    return E_NO_ERROR;
  }
}

uint16_t getBatteryVoltage(void){
  return adc.readADC_SingleEnded(0)/adc.getGain()/8;
}

uint16_t getChargerVoltage(void){
  return adc.readADC_SingleEnded(1)/adc.getGain()/8;
}

 chargeController* initCharger(){
  chargeController *A;
  A = new chargeController(
        CHARGE_PWM_PIN,
        CHARGE_PWM_INVERTED,
        CHARGE_ENABLE_PIN,
        CHARGE_ENABLE_INVERTED
      );
      return A;
}

void setup(){
  digitalWrite(PIN_POWER_HOLD, 1^POLARITY_PIN_POWER_HOLD);
  pinMode(PIN_POWER_HOLD, OUTPUT);

  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // init charger
  chargeController* CC = initCharger();
  // set callback functions
  CC->setBatteryVoltageCB(getBatteryVoltage);
  CC->setChargerVoltageCB(getChargerVoltage);
  // set limits
  CC->setVoltageDead(9700);
  CC->setVoltageBeginCharge(12000);
  CC->setVoltageTerminateCharge(13800);
  CC->setChargeCurrentMax(700);
  CC->setTemperatureMax(50);


  Serial.begin(115200);
  initCAN(CAN_500KBPS);
  initADC(GAIN_ONE);
}

uint8_t adcRead(void){
  s_adcCfg *p;
  uint16_t scratchpad;

  uint8_t returnValue = 0;

  for (uint8_t channel=0; channel<4; channel++){
    getADCmillivolts(&(scratchpad), channel);
    p = &(serviceData.adcChannelConfig[channel]);
    switch(channel){
      case ADC_VBAT_CHANNEL:
        scratchpad *= (p->rp+p->rn)/p->rn;
        if (scratchpad != processData.vBatteryMillivolt){
          ++returnValue;
          processData.vBatteryMillivolt = scratchpad;
        }
        break;
      case ADC_VCHG_CHANNEL:
        scratchpad *= (p->rp+p->rn)/p->rn;
        if (scratchpad != processData.vChargerMillivolt){
          ++returnValue;
          processData.vChargerMillivolt = scratchpad;
        }
        break;
      case ADC_IBAT_CHANNEL:
        scratchpad *= 1000/100; // mA/mV
        if (scratchpad != processData.iBatteryMilliamp){
          ++returnValue;
          processData.iBatteryMilliamp = scratchpad;
        }
        break;
      case ADC_TBAT_CHANNEL:
        processData.tBatteryCelsius = scratchpad; // TBD
        break;
      default:
        break;
    }
  }
  return returnValue;
}


void doWork(void){

  // get ADC values
  if (adcRead())
    dataChange = 1;

  //chargeControl();



}


void loop(){
  uint32_t lastRun;
  static uint32_t lastTransmission= millis();

  lastRun = millis();

  doWork();

  // send PDO
  // if dataChanged or max PDO time elapsed
  if(dataChange || (millis()-lastTransmission >= MAX_PDO_PERIOD)) {
    dataChange = false;
    lastTransmission = millis();
    CAN.sendMsgBuf(
      0x180+NODE_ID,
      0,
      sizeof(processData),
      (byte*)(&processData)
    );
  }

  while( (millis()-lastRun) < LOOP_TIME)
    ;
=======
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



>>>>>>> 7a67c6e417c95918f84fa9c0a225ee67cc05a434
}
