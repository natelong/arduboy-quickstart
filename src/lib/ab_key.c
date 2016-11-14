#include "ab.h"

ab_KeyState key;

void ab_key_init(void) {
    memset(&key, 0x00, sizeof(ab_KeyState));

    mini_pinModeInputPullup(&mini_KEY_L);
    mini_pinModeInputPullup(&mini_KEY_R);
    mini_pinModeInputPullup(&mini_KEY_U);
    mini_pinModeInputPullup(&mini_KEY_D);
    mini_pinModeInputPullup(&mini_KEY_A);
    mini_pinModeInputPullup(&mini_KEY_B);
}

void ab_key_update(void) {
    key.last     = key.current;
    key.current  = ((~PINF) & 0xf0);      // up, down, left, right
    key.current |= ((~PINE) & 0x40) >> 3; // A
    key.current |= ((~PINB) & 0x10) >> 2; // B
}

uint8_t ab_key_getCurrent(void) {
    return key.current;
}

uint8_t ab_key_getPressed(void) {
    return (key.last ^ key.current) & ~key.last;
}

uint8_t ab_key_getReleased(void) {
    return (key.last ^ key.current) & ~key.current;
}
