#pragma once

#include <Arduino_mini.h>
#include "common.h"

#define KEY_REPEAT_CNT 10

enum {
    KEY_A = _BV(3),
    KEY_B = _BV(2),
    KEY_L = _BV(5),
    KEY_R = _BV(6),
    KEY_U = _BV(7),
    KEY_D = _BV(4),

    DPAD  = (KEY_U | KEY_D | KEY_L | KEY_R),
};

typedef struct {
    uint8_t cnt;
    uint8_t trg;
    uint8_t off;
    uint8_t rep;
    int8_t repCnt;
} ST_KEY;

void KeyInit(void);
void KeyLoop(void);

uint8_t KeyGetCnt(void);
uint8_t KeyGetTrg(void);
uint8_t KeyGetOff(void);
uint8_t KeyGetRep(void);
