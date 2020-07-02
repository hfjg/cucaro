/** Hardware definition file
**/
#ifndef __hardware_h__
#define __hardware_h__

#define PWR_SWITCH_CHARGER 5
#define CHARGER_PWM 6
#define PIN_POWER_HOLD 7
#define PIN_BUTTON 8

// ADC channels and dividers
#define ADS_V_CHARGER 0
#define ADS_V_BATTERY 1
#define ADS_I_BATTERY 2
#define ADS_T_BATTERY 3

typedef struct {
  uint8_t rp;
  uint8_t rn;
  float unitConversionFactor;
}s_adc;

const s_adc adcParams[4] PROGMEM ={
  {68, 10},
  {33, 10},
  {10, 10},
  {10, 10}, ///// TBD
};

void hardwareInit(void);

#endif
