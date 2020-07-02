// pin definitions and functions
#include <Arduino.h>

typedef struct{
  uint8_t pinNumber;
  uint8_t pin_type;
  uint8_t inverted;
}s_pin;

enum pinNames{PIN_BUTTON, PIN_CHARGE_ENABLE};

s_pin button={6, INPUT_PULLUP, false};

//class 
