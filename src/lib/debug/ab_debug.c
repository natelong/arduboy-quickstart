#include "../ab.h"

static uint32_t _counters[] = {0, 0, 0, 0, 0, 0, 0, 0};

void ab_debug_increment(uint8_t i) {
    _counters[i]++;
}

void ab_debug(void) {
    ab_oled_clear();
    for (uint32_t i = 0; i < 8; i++) {
        ab_oled_setCursor(0, i);
        ab_oled_drawNumber(i);
        ab_oled_drawString(": ");
        ab_oled_drawNumber(_counters[i]);
    }
}