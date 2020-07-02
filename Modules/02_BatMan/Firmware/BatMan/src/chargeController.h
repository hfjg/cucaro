/*
 chargeController.h - Simple example in creating your own Arduino Library

 Methods:

 Blink(pin) - Constructor. Specify pin to blink
 blink(value) - Enable blinking. Accepts 1, 0 or ON and OFF
*/

#ifndef __chargeController_h__
#define __chargeController_h__

#include <Arduino.h>

class chargeController{
public:
    //Constructor. attach chargeController
    chargeController(
      uint8_t pwmPin,
      uint8_t pwmInverted,
      uint8_t enPin,
      uint8_t enInverted
    );

    void setBatteryVoltageCB( uint16_t (*f)(void)  );
    void setBatteryCurrentCB( uint16_t (*f)(void)  );
    void setChargerVoltageCB( uint16_t (*f)(void)  );
    void setBatteryTemperatureCB( int8_t (*f)(void)  );


    void setVoltageDead(uint16_t millivolt);
    void setVoltageBeginCharge(uint16_t millivolt);
    void setVoltageTerminateCharge(uint16_t millivolt);
    void setChargeCurrentMax(uint16_t milliamp);
    void setTemperatureMax(int8_t celsius);

    uint8_t charge(uint16_t iBattery, uint16_t vCharger);
    uint8_t getPWM(void);

    enum chargerState{
      CH_NO_CHARGER,
      CH_BATTERY_DEAD,
      CH_CHARGING,
      CH_DISCHARGING,
    };

private:
    uint8_t _enPin, _pwmPin, _enInverted, _pwmInverted;
    uint8_t _chgState, _pwmValue;


    uint16_t _vBattery, _vCharger, iBattery;
    int8_t _tBattery;

    uint16_t _vBatteryDead,
             _vBeginCharge,
            _vTerminateCharge,
            _iChargeMax;
    int8_t _tBatteryMax;

    uint16_t (*_batteryVoltageCB)(void);
    uint16_t (*_chargerVoltageCB)(void);
    uint16_t (*_batteryCurrentCB)(void);
    int8_t (*_batteryTemperatureCB)(void);


    void _setChargerPWM(uint8_t n);
};

#endif
