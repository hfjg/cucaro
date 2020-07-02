/*********************************************************
*
*********************************************************/
#include <Arduino.h>
#include "hardware.h"
#include "ctwi.h"
#include "charge.h"

int16_t charge(
  s_i2cReadRegs *rr,
  s_i2cControlRegs *cr
){
  static uint8_t chargePWM = 255;

  // check for charger
    rr->state.flag.chargerConnected =
          (rr->vCharger >= cr->vChargerMin)
      &&  (rr->vCharger <= cr->vChargerMax);


  // switch charging circuit off if no charger present
  if (!rr->state.flag.chargerConnected){
    rr->state.flag.charging = 0;
    digitalWrite(PWR_SWITCH_CHARGER, 0);
    // safety measure: set lowest charging voltage
    analogWrite(CHARGER_PWM, 255);
  }
  else if (!rr->state.flag.charging){
    // not (yet) charging
    rr->state.flag.charging =
      (rr->vBattery <= cr->voltageChargeBegin);
    // activate charge
    if (rr->state.flag.charging){
      digitalWrite(PWR_SWITCH_CHARGER, 1);
      // safety measure: set lowest charging voltage
      analogWrite(CHARGER_PWM, 255);
    }
  }
  else { // flag charging seat, meaning charging in progress
    if (rr->vBattery >= cr->voltageMax){
      // terminate charge
      rr->state.flag.charging = 0;
      // set charger off
      digitalWrite(PWR_SWITCH_CHARGER, 0);
      // safety measure: set lowest charging voltage
      analogWrite(CHARGER_PWM, 255);
    }
  }

  // control charge current
  if (rr->state.flag.charging){
    if (
            // charge current too high
             (rr->iBattery > cr->iBatteryCharge)
             // battery voltage too high
          || (rr->vBattery > cr->voltageMax)
      ){
      if (chargePWM<255) analogWrite(CHARGER_PWM, ++chargePWM);
    }
    else{
      if (chargePWM>0) analogWrite(CHARGER_PWM, --chargePWM);
    }
    return chargePWM;
  }
  else{
    return 0;
  }
}
