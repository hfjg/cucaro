# BatMan Memos

## Current measurement
Sensor will be ACS-712 analog sensor.

Output: U_{ACS}=2500mV + k * I

where 
* k=185mV/A for ACS712-5,
* k=100mV/A for ACS712-20 and 
* k=66mV/A for ACS712-30

For the ADC values this yields:
n_{ADC} = (2^N-1) * U_{ADC}/U_{Ref}

and for the (reverse) sensitivity:
dI/dn = U_{Ref} / (k * (2^N-1) )
