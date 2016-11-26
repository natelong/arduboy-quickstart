#include "ab_debug.h"
#include "../screen/ab_screen.h"

static uint32_t _counters[] = {0, 0, 0, 0, 0, 0, 0, 0};

void ab_debug_increment(uint8_t i) {
    _counters[i]++;
}

void ab_debug(void) {
    ab_screen_clear();
    for (uint32_t i = 0; i < 8; i++) {
        ab_screen_setCursor(0, i);
        ab_screen_drawNumber(i);
        ab_screen_drawString(": ");
        ab_screen_drawNumber(_counters[i]);
    }
}