# BatMan
Battery management module for CuCaRo

## Components
* Arduino Pro Mini
* CAN-Interface with MCP2515
* 16-Bit ADC-Module ADS1115
* Current Sensor ACS712
* Temperature Sensor (type tbd)
* 2x LM2596, one for VECU (5V), one for charger

## Firmware functions
- Module attaches as I2C-Slave or as CAN-Module (tbd)
- Periodically measure voltages (10/s),  current (10/s), batteryTemperature (1/s)
- Check for charger, charge if necessary (battery type and data in setup)