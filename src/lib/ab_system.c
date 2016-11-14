#include <avr/power.h>
#include <avr/sleep.h>
#include "ab.h"

void ab_init(void) {
    power_timer2_disable();
    power_adc_disable();
    power_twi_disable();
    power_usart0_disable();
    power_usart1_disable();

    ab_oled_init();
    FrameInit();
    ab_sound_init();
    ab_key_init();
}

void ab_idle(void) {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
}

bool ab_loopStart(void) {
    if(FrameLoop() == false) {
        return false;
    }

    ab_key_update();
    return true;
}

void ab_loopEnd(void) {
    ab_oled_display();
    ab_oled_clear();
}
