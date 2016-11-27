#include "ab_sound.h"

const uint16_t noteTable[64] PROGMEM = {
  0,
  262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,
  523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,
  8372, 8870, 9397,
};

// 0x03: put timer into waveform generation mode 4: CTC w/ TOP = OCRnA
// 0x01: clock select bit to "no prescaling"
#define INITIAL_PIN_MODE (0x08 | 0x01)

// 0x02: OCIEnA
#define INTERRUPT_MASK (0x02)

const uint8_t AB_CHANNEL_1 = 0;
const uint8_t AB_CHANNEL_2 = 1;

void ab_sound_init(void) {
    { // Set sound pins to output mode
        uint8_t oldSREG = SREG;
        cli();
        DDRC |= (1 << 6);
        DDRC |= (1 << 7);
        SREG = oldSREG;
    }

    TCCR3A = 0;
    TCCR1A = 0;

    TCCR3B = INITIAL_PIN_MODE;
    TCCR1B = INITIAL_PIN_MODE;

    power_timer3_enable();
    power_timer1_enable();
}

void ab_sound_playNote(uint8_t channel, uint8_t note) {
    uint16_t frequency = pgm_read_word(noteTable + note);
    ab_sound_playTone(channel, frequency);
}

void ab_sound_playTone(uint8_t channel, uint16_t frequency) {
    uint32_t ocr = F_CPU / frequency - 1;
    uint8_t  pre = 1;

    if (ocr > 0xffff) {
        ocr = F_CPU / frequency / 64 - 1;
        pre = 3;
    }

    if (channel == AB_CHANNEL_1) {
        TCCR1B = (TCCR1B & 0xf8) | pre;
        OCR1A  = ocr;
        TIMSK1 |= (1 << OCIE1A);
    } else if (channel == AB_CHANNEL_2) {
        TCCR3B = (TCCR3B & 0xf8) | pre;
        OCR3A  = ocr;
        TIMSK3 |= (1 << OCIE3A);
    }
}

void ab_sound_stopChannel(uint8_t channel) {
    if (channel == AB_CHANNEL_1) {
        TIMSK1 &= ~INTERRUPT_MASK;
    } else if (channel == AB_CHANNEL_2) {
        TIMSK3 &= ~INTERRUPT_MASK;
    }
}

// TIMER 3 ch0
ISR(TIMER3_COMPA_vect) {
    PORTC ^= (1 << 6);
}

// TIMER 1 ch1
ISR(TIMER1_COMPA_vect) {
    PORTC ^= (1 << 7);
}
