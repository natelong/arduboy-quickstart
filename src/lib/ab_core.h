#pragma once

#include "ab_common.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/power.h>
#include <avr/power.h>
#include <avr/sleep.h>

void     mini_init(void);
uint32_t mini_millis(void);
uint32_t mini_micros(void);
void     mini_delay(uint32_t);
