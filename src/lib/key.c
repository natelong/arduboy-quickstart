#include "Arduino_mini.h"
#include "key.h"


ST_KEY Key;



void KeyInit(void) {
    _Memset(&Key, 0x00, sizeof(ST_KEY));

    mini_pinModeInputPullup(&mini_KEY_L);
    mini_pinModeInputPullup(&mini_KEY_R);
    mini_pinModeInputPullup(&mini_KEY_U);
    mini_pinModeInputPullup(&mini_KEY_D);
    mini_pinModeInputPullup(&mini_KEY_A);
    mini_pinModeInputPullup(&mini_KEY_B);
}

void KeyLoop(void) {
    uint8_t cnt;

    cnt  = ((~PINF) & 0xf0);      // up, down, left, right
    cnt |= ((~PINE) & 0x40) >> 3; // A
    cnt |= ((~PINB) & 0x10) >> 2; // B

    Key.trg = (Key.trg ^ cnt) & ~Key.cnt;
    Key.off = ~cnt & Key.cnt;
    Key.cnt = cnt;

    if(Key.trg & DPAD || Key.repCnt == 0) {
        Key.rep = Key.cnt;
        Key.repCnt = KEY_REPEAT_CNT;
    } else {
        Key.rep = 0;
    }

    if(Key.cnt & DPAD) {
        if(Key.repCnt != 0) Key.repCnt--;
    } else {
        Key.repCnt = 0;
    }
}

uint8_t KeyGetCnt(void) {
    return Key.cnt;
}

uint8_t KeyGetTrg(void) {
    return Key.trg;
}

uint8_t KeyGetOff(void) {
    return Key.off;
}

uint8_t KeyGetRep(void) {
    return Key.rep;
}
