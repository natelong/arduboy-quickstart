#pragma once

#include "ab_common.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/power.h>
#include <avr/power.h>
#include <avr/sleep.h>

typedef struct {
    volatile uint8_t* mode;
    volatile uint8_t* out;
             uint8_t  mask;
} mini_Pin;

void     mini_init(void);
uint32_t mini_millis(void);
uint32_t mini_micros(void);
void     mini_delay(uint32_t);
void     mini_pinModeOutput(const mini_Pin* pin);
void     mini_setPinHigh(const mini_Pin* pin);
