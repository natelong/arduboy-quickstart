#include "Arduino_mini.h"
#include <avr/power.h>
#include "snd.h"

// 0x03: put timer into waveform generation mode 4: CTC w/ TOP = OCRnA
// 0x01: clock select bit to "no prescaling"
#define INITIAL_PIN_MODE (0x08 | 0x01)

// 0x02: OCIEnA
#define INTERRUPT_MASK (0x02)

const ab_Channel ab_Channel_1 = {&TIMSK1, &OCR1A};
const ab_Channel ab_Channel_2 = {&TIMSK3, &OCR3A};

void ab_sound_init(void) {
    mini_pinModeOutput(&mini_SND_1);
    mini_pinModeOutput(&mini_SND_2);

    TCCR3A = 0;
    TCCR1A = 0;

    TCCR3B = INITIAL_PIN_MODE;
    TCCR1B = INITIAL_PIN_MODE;

    power_timer3_enable();
    power_timer1_enable();
}

void ab_sound_playNote(const ab_Channel* channel, uint16_t frequency) {
    *channel->source = F_CPU / frequency - 1;
    *channel->mask |= INTERRUPT_MASK;
}

void ab_sound_stopNote(const ab_Channel* channel) {
    *channel->mask &= ~INTERRUPT_MASK;
}

// TIMER 3 ch0
ISR(TIMER3_COMPA_vect) {
    *mini_SND_1.out ^= mini_SND_1.mask;
}

// TIMER 1 ch1
ISR(TIMER1_COMPA_vect) {
    *mini_SND_2.out ^= mini_SND_2.mask;
}
