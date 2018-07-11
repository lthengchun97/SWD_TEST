#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdint.h>
#include "controlandstatus.h"

typedef struct SWDIO_PIN_INIT SWDIO_PIN_INIT;

struct SWDIO_PIN_INIT{
  DP_ABORT *abort;
  DP_REG_STAT *stat;
  int pin;
  int mode;
  int round;
  int req_IDCODE;
  uint8_t acknowledge;
  uint32_t outdata;
  uint32_t indata;
  uint32_t lastdata;
};





#endif // _STRUCT_H
