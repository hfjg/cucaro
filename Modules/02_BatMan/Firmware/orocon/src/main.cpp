#include <Arduino.h>

#define PDO_LOOP_TIME 1000

#define CANOPEN_DEVICE_ID 2

typedef union{
  struct{
    uint8_t flag;
    uint16_t vBatteryMillivolt;
    uint16_t vChargerMillivolt;
    int16_t  iBatteryMilliamp;
  };
  uint8_t bytes[8];
}s_TXPDO0;

typedef union{
  struct{
    uint8_t requestVoltages: 1;
    uint8_t requestShutdown : 1;
  }flag;
  uint8_t bytes[1];
}s_RXPDO0;

#include <Adafruit_ADS1015.h>


 Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

/**
  \todo define CAN Frames and addresses
**/
#include <CAN.h>
s_RXPDO0 RXPDO0;
s_TXPDO0 TXPDO0; //, *pu8;
uint8_t *pu8;



enum e_errors {E_NO_ERROR, E_INVALID_PARAMETER, E_SWIRE_ERROR};
enum e_buttonPresses {BP_NONE, BP_SHORT, BP_LONG};

e_buttonPresses buttonPress(void){

  return BP_NONE;
}


uint8_t adcRead(uint8_t channel, uint16_t *conversionResult){

  if (channel > 3) return E_INVALID_PARAMETER;

  return E_NO_ERROR;
}


void canCommunication(){
uint32_t lastHeartbeat;

  if (CAN.parsePacket()){
    // we received a packet
    int packetID = CAN.packetId();

    switch(packetID){
      case 0x200+CANOPEN_DEVICE_ID:
        // it is "our" RXPDO0
        pu8 = &(RXPDO0.bytes[0]);
        while (CAN.available()) {
          *pu8++ = CAN.read();
        }
        lastHeartbeat = millis();
      break;
      // cases SYNC, EMCY
      default:
        // unexpected, flush buffer
        while (CAN.available())CAN.read();
        break;
    }
  }

  if (millis()-lastHeartbeat > 10000){
    RXPDO0.flag.requestShutdown = 1;
  }

  // transmit data if requested
  if (RXPDO0.flag.requestVoltages){
    RXPDO0.flag.requestVoltages = 0;

    // CAN send PDO0
    CAN.beginPacket(0x180+CANOPEN_DEVICE_ID, sizeof(s_TXPDO0));
    pu8 = &(TXPDO0.bytes[0]);
    for (uint8_t i=0; i<sizeof(s_TXPDO0); i++){
      CAN.write(*pu8++);
    }
    CAN.endPacket();
  }
}

void setup() {

  Wire.begin();

  //! Start CAN bus at 250 kBps
  CAN.begin(250E3);

// main loop
  for (;;){

    // get adc values and update PDO
    canCommunication();

    // check button
    uint8_t bp = buttonPress();
    switch(bp){
      case BP_SHORT:
        // initiate power down
        break;
      case BP_LONG:
        // initiate reboot
        break;
      default:
        break;
    }

    // charge control
  }
}

void loop() {
}
