#pragma once

#include "../ab_common.h"

void     ab_init(void);
uint32_t ab_millis(void);
uint32_t ab_micros(void);
void     ab_delay(uint32_t);
void     ab_frame(void);
void     ab_setLED(uint8_t red, uint8_t green, uint8_t blue);
void     ab_reset(void);