
#include <Arduino.h>
#include <Adafruit_ADS1015.h>

#include "adc.h"
#include "errors.h"

Adafruit_ADS1115 ads;

/**
* \brief ADC initialization
*
* \author Hermann Gebhard
* \copyright (C)2020 H. Gebhard
* \todo implement other gains
**/
void adcInit(uint8_t gain){
  ads.begin();
  if (gain==1) ads.setGain(GAIN_ONE); // 1x gain   +/- 4.069V  1 bit = 0.125mV
}


/**
* \brief ADC raw read and convert to millivolts.
*       ADC Gain is implemented
*
* \author Hermann Gebhard
* \copyright (C)2020 H. Gebhard
**/
uint16_t getADCmillivolts(uint8_t channel){
    return ads.readADC_SingleEnded(channel)/ads.getGain()/8;
}


/**
* \brief ADC voltage measurement with voltage divider.
*     rp to positive side and rn to Ground.
*     Only resistor ratio is needed.
*
* \author Hermann Gebhard
* \copyright (C)2020 H. Gebhard
**/
uint8_t getVoltageMillivolts(uint16_t *result, uint8_t channel, uint8_t rp, uint8_t rn){
    if ( (channel > 3) || (rn == 0) )
      return E_INVALID_PARAMETER;

    *result = (rp+rn)/rn*getADCmillivolts(channel);
    return E_NO_ERROR;
}

/**
* \brief ADC current measurement over ACS712
*
* \author Hermann Gebhard
* \copyright (C)2020 H. Gebhard
**/
uint8_t getCurrentACS712(int16_t *result,
    uint8_t channel, uint8_t iMax){
    // V_USB = 0.125mV/GAIN = 1mV/(8*GAIN)
    switch(iMax){
      case 5:
        *result = (getADCmillivolts(channel)-2500) * 1000/185;
        return E_NO_ERROR;
      case 20:
        *result = (getADCmillivolts(channel)-2500) * 1000/100;
        return E_NO_ERROR;
      case 30:
        *result = (getADCmillivolts(channel)-2500) * 1000/66;
        return E_NO_ERROR;
      default:
        *result = 0;
        return E_INVALID_PARAMETER;
    }
    // should never get here
    return E_INVALID_PARAMETER;
}
