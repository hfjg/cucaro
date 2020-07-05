#On-Off-Unit
The on-off-unit ensures that the Raspberry Pi is switched off correctly, i.e. it can do ist shutdown process.

## Specification
* One button operation 
    * If off, button press turns on SBC Power Supply
    * If on, short button press initiates reset, long press initiates shutdown

* Additional functions
    * Battery Charge Control
    * Buffered internal battery to initiate shutdown on battery fail during operation
    * Measurement of 
        * Battery voltage
        * Battery Current
        * Charger voltage 
      and option to readout values via I2C.


## Realisation
Microcontroller communicates with SBC via I2C.