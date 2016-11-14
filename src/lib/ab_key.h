#pragma once

#include "Arduino_mini.h"
#include "ab_common.h"

enum {
    AB_KEY_A = _BV(3),
    AB_KEY_B = _BV(2),
    AB_KEY_L = _BV(5),
    AB_KEY_R = _BV(6),
    AB_KEY_U = _BV(7),
    AB_KEY_D = _BV(4),

    AB_DPAD  = (AB_KEY_U | AB_KEY_D | AB_KEY_L | AB_KEY_R),
};

typedef struct {
    uint8_t current;
    uint8_t last;
} ab_KeyState;

void    ab_key_init(void);
void    ab_key_update(void);
uint8_t ab_key_getCurrent(void);
uint8_t ab_key_getPressed(void);
uint8_t ab_key_getReleased(void);
