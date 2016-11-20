#pragma once

#include "ab_common.h"

enum {
    AB_KEY_A = _BV(3),
    AB_KEY_B = _BV(2),
    AB_KEY_L = _BV(5),
    AB_KEY_R = _BV(6),
    AB_KEY_U = _BV(7),
    AB_KEY_D = _BV(4),

    AB_KEY_DPAD  = (AB_KEY_U | AB_KEY_D | AB_KEY_L | AB_KEY_R),
    AB_KEY_BTN   = (AB_KEY_A | AB_KEY_B),
    AB_KEY_ALL   = (AB_KEY_DPAD | AB_KEY_BTN)
};

void    ab_key_init(void);
void    ab_key_update(void);
uint8_t ab_key_getCurrent(void);
uint8_t ab_key_getPressed(void);
uint8_t ab_key_getReleased(void);
