# OROCON: On-Reset-Off-Controller
OROCON ensures that the Single-Board_Computer (i.e. Raspberry Pi) is shut down correctly.  Communication to SBC is via I2C.

## Specification
* One button operation 
    * If off, button press turns on SBC Power Supply
    * If on, short button press initiates reset, long press initiates shutdown

* Additional functions
    * SLA Battery Charge Control
    * Buffered internal battery to initiate shutdown on battery fail during operation
    * Measurement of 
        * Battery voltage
        * Battery Current
        * Charger voltage 
      and option to readout values via I2C.


## Connectors
* Battery in (XT60)
* Charger in (DC Barrel Jack)

