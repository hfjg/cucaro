#include <Arduino.h>
#include <eXoCAN.h>
eXoCAN can;
#define CANOPEN_NODE_ID 2

#define PIN_vBattery 10
#define PIN_iBattery 11
#define PIN_vCharger 12
#define PIN_CHARGER_ENABLE 6

#define TXPDO0_ID 0x180+CANOPEN_NODE_ID
#define RXPDO0_ID 0x200+CANOPEN_NODE_ID
#define EMCY_ID   0x080+CANOPEN_NODE_ID
#define RXSDO_ID  0x600+CANOPEN_NODE_ID
#define bluePillLED PC13

#define MEASURE_LOOP_TIME 1000
#define digitalToggle(x)  digitalWrite(x, !digitalRead(x))

enum e_keyPress {NONE, SHORT, LONG, UNDEF};
enum e_emergencies {EMCY_BATTERY_DEAD, EMCY_CHARGER_OVERVOLTAGE, EMCY_CHARGER_FAIL};

typedef struct{
    struct{
      uint8_t chargerConnected : 1;
      uint8_t charging : 1;
      e_keyPress keypress : 2;
    }
    flag;
    uint16_t vBatteryMillivolts;
    int16_t iBatteryMilliamps;
    uint16_t vChargerMillivolts;
}s_TXPDO;
s_TXPDO txpdo;

uint8_t CANrxBytes[8];

struct{
  uint16_t vBatteryMin, vBatteryMax, vBatteryStartCharge;
  uint16_t vChargerMin, vChargerMax;

}sdo;

void emergency (uint8_t emcyNumber){

  // can.transmit(txMsgID, txData, txDataLen);
  can.transmit(EMCY_ID, &emcyNumber, sizeof(uint8_t));
}

void canISR() // get CAN bus frame passed by a filter into fifo0
{ int id=RXPDO0_ID;
  int fltIdx=0;

  can.receive(id, fltIdx, CANrxBytes);  // empties fifo0 so that another another rx interrupt can take place
}

void setup() {
  // put your setup code here, to run once:
  digitalWrite(PIN_CHARGER_ENABLE, LOW);
  pinMode(PIN_CHARGER_ENABLE, OUTPUT);

  can.begin(STD_ID_LEN, BR250K, PORTB_8_9_WIRE_PULLUP); // 11b IDs, 250k bit rate, no transceiver chip, portA pins 11,12
  can.filterMask16Init(0, 0, 0x7ff, 0, 0);                // filter bank 0, filter 0: don't pass any, flt 1: pass all msgs
  pinMode(bluePillLED, OUTPUT);

  can.filterList16Init(2, RXPDO0_ID, RXSDO_ID, 0x71, 0x72);  // rx bank 2. passes IDs == 5, 10, 0x71 and 0x72

  can.attachInterrupt(canISR);
  // can.filterMask16Init(0, rxMsgID, 0x7ff);   // filter bank 0, filter 0: allow ID = rxMsgID, mask = 0x7ff (must match)
                                                // without calling a filter, the default passes all IDs
}

void measure (void){
  txpdo.vBatteryMillivolts = analogRead(PIN_vBattery);
  txpdo.iBatteryMilliamps  = analogRead(PIN_iBattery);
  txpdo.vChargerMillivolts = analogRead(PIN_vCharger);

  txpdo.flag.chargerConnected =
        (txpdo.vChargerMillivolts >= sdo.vChargerMin)
    &&  (txpdo.vChargerMillivolts <= sdo.vChargerMax);

  if (txpdo.vBatteryMillivolts < sdo.vBatteryMin){
    emergency(EMCY_BATTERY_DEAD);
  }
  else if (txpdo.vChargerMillivolts > sdo.vChargerMax){
    emergency(EMCY_CHARGER_OVERVOLTAGE);
  }

  else if (!txpdo.flag.charging){
    if (    (txpdo.flag.chargerConnected)
        &&  (txpdo.vBatteryMillivolts <= sdo.vBatteryStartCharge)
        &&  (txpdo.vBatteryMillivolts >= sdo.vBatteryMin)
    ){
      txpdo.flag.charging = 1;
      digitalWrite(PIN_CHARGER_ENABLE, HIGH);
      }
  }
  else{
  // charging is active
    if (txpdo.vBatteryMillivolts >= sdo.vBatteryMax){
      // battery is full, terminate charge
      txpdo.flag.charging = 0;
      digitalWrite(PIN_CHARGER_ENABLE, LOW);
    }
    else if (txpdo.vChargerMillivolts <= sdo.vChargerMin){
      // charger failed
      txpdo.flag.charging = 0;
      digitalWrite(PIN_CHARGER_ENABLE, LOW);
      emergency(EMCY_CHARGER_FAIL);
    }
  }


}


void loop() {
  uint32_t now;
  static uint32_t lastRun = 0;

  int id, fltIdx;


  now = millis();
  if (now - lastRun > MEASURE_LOOP_TIME){
    lastRun = now;
    measure();
  }

  // check for CAN received frames
  if (can.receive(id, fltIdx, CANrxBytes) > -1) // poll for rx
      digitalToggle(bluePillLED);

  //processRXPDO();
  //processSDO();
  //processMISC();

}
