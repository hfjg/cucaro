#include <Arduino.h>
#include "keys.h"



uint8_t checkKey(uint8_t keyPin){
  enum e_keyStates {S_IDLE, S_PRESSED, S_RELEASED};

  static uint8_t count = 0;
  static e_keyStates state = S_IDLE;
  uint8_t retval;

  retval = KEY_NONE;

  switch(state){
    case S_PRESSED:
      ++count;
      if (digitalRead(keyPin)==1)
        state = S_RELEASED;
      break;

    case S_RELEASED:
      if (count < 3){
      }
      else if (count < 15){
        retval = KEY_SHORT;
      }
      else {
        retval = KEY_LONG;
      }
      break;

      case S_IDLE:
      default:
        count = 0;
        if (digitalRead(keyPin)==0)
          state = S_PRESSED;
        break;
  }
  return retval;
}
