#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdarg.h>

#include "Arduino_mini.h"

#ifdef __cplusplus
extern "C"{
#endif

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif