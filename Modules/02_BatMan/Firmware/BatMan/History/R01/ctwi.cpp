/** cucaro twi funtions
**/
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#include "ctwi.h"
#include "battery.h"
#include "helpers.h"

uint8_t *twiPtr;
uint32_t shutdownTimer = millis();


s_i2cReadRegs *readRegs;
s_i2cControlRegs *ctlRegs;

/**
* \brief Callback function for TWI data requests.
*
* Auto shutdown timer is reset
*
* Number of transmitted bytes are limited to size of data structure
**/
void TWIrequest(void){
  uint16_t maxBytes = sizeof(s_i2cReadRegs) - (uint16_t)(twiPtr)-(uint16_t)(readRegs) ;
  Wire.write(twiPtr, maxBytes);
  shutdownTimer = millis();
}

/**
* \brief Callback function for TWI data reception.
*
*
**/
void TWIreceive(int bytesReceived){
  if (bytesReceived-- > 0){
    twiPtr = (uint8_t *)(ctlRegs +  Wire.read());
    shutdownTimer = millis();
  }

  while ( bytesReceived--
        && twiPtr<(uint8_t *)(ctlRegs+sizeof(s_i2cControlRegs)) )
    *twiPtr++ = Wire.read();

  // discard received bytes beyond memory
  while ( bytesReceived--)
    Wire.read();
}



/**
* \brief Initialization of TWI data structures
*
*
**/void ctwiInit(s_i2cReadRegs *rr, s_i2cControlRegs *cr){
  uint16_t c;

  Wire.begin(CUCARO_SLAVE_ADDRESS);
  Wire.onRequest(TWIrequest);
  Wire.onReceive(TWIreceive);
  readRegs = rr;
  ctlRegs = cr;

  // initialize read registers
  rr->state.complete = 0;

  // initialize command registers
  cr->command.shutdown = 0;
  EEPROM.get(0, cr);
  EEPROM.get(sizeof(s_i2cControlRegs), c);

  if ( c != checksum((uint8_t *)(cr), sizeof(s_i2cControlRegs))){
    // init structure to defaults
    cr->batteryType = BAT_SEALED_LEAD_ACID; // SLA
    cr->numCells = 6; // 12V nominal
    cr->capacityMilliAmpHour = 7000;
    cr->sampleTimeMillis = 500;

    // automatic setup rest of battery data
    batterySetup(cr);

    // save structure to EEPROM
    EEPROM.put(0, cr);
    EEPROM.put(sizeof(s_i2cControlRegs), c);
  }
}
