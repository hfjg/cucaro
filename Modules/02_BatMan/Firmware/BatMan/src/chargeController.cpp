/* charge Control
*
*/

#include <Arduino.h>
#include "chargeController.h"

/**
  Constructor
**/
chargeController::chargeController(
  uint8_t pwmPin,
  uint8_t pwmInverted,
  uint8_t enPin,
  uint8_t enInverted
){
  _enPin = enPin;
  _pwmPin = pwmPin;
  _enInverted = enInverted;
  _pwmInverted = pwmInverted;

  // disable charger
  _setChargerPWM(0);
  if (_enPin != 0) pinMode(_enPin, OUTPUT);

  // switch PWM off
  digitalWrite(_pwmPin, 0^_pwmInverted);
  pinMode(_pwmPin, OUTPUT);
}


void chargeController::_setChargerPWM(uint8_t n){
  if (n==0){
    if (_enPin != 0) digitalWrite(_enPin, 0^_enInverted);

    if (_pwmInverted)
      analogWrite(_pwmPin, 255);
    else
      analogWrite(_pwmPin, 0);
  }
  else{
    if (_pwmInverted) n = 255-n;
    if (_enPin != 0) digitalWrite(_enPin, 1^_enInverted);
    analogWrite(_pwmPin, n);
  }
}

/**
  \return current PWM value (mostly for testing)
**/
uint8_t chargeController::getPWM(void){
  return _pwmValue;
}

/**
  \brief Set callback function to measure battery voltage.

  Function should return battery voltage in millivolts
**/
void chargeController::setBatteryVoltageCB( uint16_t (*f)(void)  ){
  _batteryVoltageCB = f;
}

/**
  \brief Set callback function to measure battery voltage.

  Function should return battery voltage in millivolts
**/
void chargeController::setChargerVoltageCB( uint16_t (*f)(void)  ){
  _chargerVoltageCB = f;
}

/**
  \brief Set callback function to measure battery current.

  Function should return battery current in milliamps
**/
void chargeController::setBatteryCurrentCB( uint16_t (*f)(void)  ){
  _batteryCurrentCB = f;
}

/**
  \brief Set callback function to measure battery temperature.

  Function should return battery temperature in celsius
**/
void chargeController::setBatteryTemperatureCB( int8_t (*f)(void)  ){
  _batteryTemperatureCB = f;
}


uint8_t chargeController::charge(uint16_t iBattery, uint16_t vCharger){
static uint16_t charging = 0;

  // get momentary values
 _vBattery = _batteryVoltageCB();
 _vCharger = _chargerVoltageCB();

  if (vCharger < _vTerminateCharge+3000){
    _setChargerPWM(0);
    return CH_NO_CHARGER;
  }

  if (_vBattery < _vBatteryDead){
    _setChargerPWM(0);
    return CH_BATTERY_DEAD;
  }


  if (charging){
    // terminate charge
    if (    (_vBattery > _vTerminateCharge)
        ||  (_batteryTemperatureCB() > _tBatteryMax)
    ){
      charging = 0;
    }
    else{
      // continue charging
      // check voltage and current
      while (   (_batteryVoltageCB() <= _vTerminateCharge)
             && (_batteryCurrentCB() < _iChargeMax)
             && (_pwmValue < 255)
      ) _setChargerPWM(++(_pwmValue));

      while ( (  (_batteryVoltageCB() > _vTerminateCharge)
              || (_batteryCurrentCB() > _iChargeMax)
            ) && (_pwmValue > 0)
      ) _setChargerPWM(--(_pwmValue));
    }
    return CH_CHARGING;
  }
  else {
    // not (yet) charging
    _setChargerPWM(0);
    if (_vBattery <= _vBeginCharge){
      charging = 1;
    }
    return CH_DISCHARGING;
  }

  return 0;
}

void chargeController::setVoltageDead(uint16_t millivolt){
  _vBatteryDead = millivolt;
}
void chargeController::setVoltageBeginCharge(uint16_t millivolt){
  _vBeginCharge = millivolt;
}
void chargeController::setVoltageTerminateCharge(uint16_t millivolt){
  _vTerminateCharge = millivolt;
}
void chargeController::setChargeCurrentMax(uint16_t milliamp){
  _iChargeMax = milliamp;
}
void chargeController::setTemperatureMax(int8_t celsius){
  _tBatteryMax = celsius;
}
