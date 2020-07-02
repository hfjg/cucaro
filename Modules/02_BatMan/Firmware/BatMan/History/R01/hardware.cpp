/****************************
* hardware initialization
******************************/
#include <Arduino.h>
#include "hardware.h"


void hardwareInit(void){
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pinMode(PIN_POWER_HOLD, OUTPUT);
  digitalWrite(PIN_POWER_HOLD, 1);

  pinMode(PWR_SWITCH_CHARGER, OUTPUT);

  digitalWrite(CHARGER_PWM, 1);
  pinMode(CHARGER_PWM, OUTPUT);
}
