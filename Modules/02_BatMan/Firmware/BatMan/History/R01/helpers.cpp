/*********************************************************
*
*********************************************************/
#include <Arduino.h>

uint16_t checksum (uint8_t *p, uint16_t numBytes){
  uint16_t sum;

  sum=0;
  while (numBytes--)
    sum += *p++;
  return sum;
}
