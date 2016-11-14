#include "ab.h"

#ifndef AB_FRAMERATE
#define AB_FRAMERATE 60
#endif

#define TARGET_MS (1000 / AB_FRAMERATE)

static uint32_t last = 0;
static uint32_t delta = 0;
static uint32_t now = 0;

void ab_init(void) {
    power_timer2_disable();
    power_adc_disable();
    power_twi_disable();
    power_usart0_disable();
    power_usart1_disable();

    ab_oled_init();
    ab_sound_init();
    ab_key_init();
}

void ab_frame(void) {
    ab_oled_display();
    ab_oled_clear();

    for (;;) {
        now = mini_millis();
        delta = now - last;
        if (delta >= TARGET_MS) {
            last = now;
            break;
        }
    }
}
