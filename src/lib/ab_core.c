#include "ab.h"

#include <avr/wdt.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#ifndef AB_FRAMERATE
#define AB_FRAMERATE 60
#endif

#define TARGET_MS (1000 / AB_FRAMERATE)

static uint32_t last = 0;
static uint32_t delta = 0;
static uint32_t now = 0;

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW ((64 * 256) / (F_CPU / 1000000L))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile uint32_t timer0_overflow_count = 0;
volatile uint32_t timer0_millis         = 0;
static   uint8_t  timer0_fract          = 0;

ISR(TIMER0_OVF_vect) {
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = timer0_millis;
    uint8_t f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}

uint32_t ab_millis() {
    uint32_t m;
    uint8_t oldSREG = SREG;

    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();
    m = timer0_millis;
    SREG = oldSREG;

    return m;
}

uint32_t ab_micros() {
    uint32_t m;
    uint8_t oldSREG = SREG, t;

    cli();
    m = timer0_overflow_count;

    t = TCNT0;

    if ((TIFR0 & _BV(TOV0)) && (t < 255))
        m++;

    SREG = oldSREG;

    return ((m << 8) + t) * (64 / (F_CPU / 1000000L));
}

void ab_delay(uint32_t ms) {
    uint8_t sreg = SREG;
    sei();
    uint32_t start = ab_micros();

    while (ms > 0) {
        while (ms > 0 && (ab_micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }
    SREG = sreg;
}

void ab_frame(void) {
    ab_oled_display();
    ab_oled_clear();
    ab_usb_update();

    for (;;) {
        now = ab_millis();
        delta = now - last;
        if (delta >= TARGET_MS) {
            last = now;
            break;
        }
    }
}

void ab_setLED(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t oldSREG = SREG;
    cli();

    if ((255 - red)   > 128) PORTB |= (1 << 6); else PORTB &= ~(1 << 6);
    if ((255 - green) > 128) PORTB |= (1 << 7); else PORTB &= ~(1 << 7);
    if ((255 - blue)  > 128) PORTB |= (1 << 5); else PORTB &= ~(1 << 5);

    SREG = oldSREG;
}




// Bootloader related fields
// Old Caterina bootloader places the MAGIC key into unsafe RAM locations (it can be rewritten
// by the running sketch before to actual reboot).
// Newer bootloaders, recognizable by the LUFA "signature" at the end of the flash, can handle both
// the usafe and the safe location. Check once (in USBCore.cpp) if the bootloader in new, then set the global
// _updatedLUFAbootloader variable to true/false and place the magic key consequently
#ifndef MAGIC_KEY
#define MAGIC_KEY 0x7777
#endif

#ifndef MAGIC_KEY_POS
#define MAGIC_KEY_POS 0x0800
#endif

#ifndef NEW_LUFA_SIGNATURE
#define NEW_LUFA_SIGNATURE 0xDCFB
#endif

#if MAGIC_KEY_POS != (RAMEND-1)
    #define LUFA_COMPAT
#endif

void ab_reset(void) {
    ab_setLED(0, 255, 0);
    uint16_t magic_key_pos = MAGIC_KEY_POS;
    bool _updatedLUFAbootloader = false;

#ifdef LUFA_COMPAT
    if (pgm_read_word(FLASHEND - 1) == NEW_LUFA_SIGNATURE) {
        _updatedLUFAbootloader = true;
    }
#endif

// If we don't use the new RAMEND directly, check manually if we have a newer bootloader.
// This is used to keep compatible with the old leonardo bootloaders.
// You are still able to set the magic key position manually to RAMEND-1 to save a few bytes for this check.
#ifdef LUFA_COMPAT
    // For future boards save the key in the inproblematic RAMEND
    // Which is reserved for the main() return value (which will never return)
    if (_updatedLUFAbootloader) {
        // horray, we got a new bootloader!
        magic_key_pos = (RAMEND-1);
    }

    // Backup ram value if its not a newer bootloader.
    // This should avoid memory corruption at least a bit, not fully
    if (magic_key_pos != (RAMEND-1)) {
        *(uint16_t *)(RAMEND-1) = *(uint16_t *)magic_key_pos;
    }
#endif

    // Store boot key
    *(uint16_t *)magic_key_pos = MAGIC_KEY;
    wdt_enable(WDTO_120MS);
    while (true) ;
}

void ab_init() {
    // this needs to be called before setup() or some functions won't work there
    sei();

    // on the ATmega168, timer 0 is also used for fast hardware pwm
    // (using phase-correct PWM would mean that timer 0 overflowed half as often
    // resulting in different ab_millis() behavior on the ATmega8 and ATmega168)
    sbi(TCCR0A, WGM01);
    sbi(TCCR0A, WGM00);
    sbi(TCCR0B, CS01); // this combination is for the standard 168/328/1280/2560
    sbi(TCCR0B, CS00);
    sbi(TIMSK0, TOIE0); // enable timer 0 overflow interrupt

    // timers 1 and 2 are used for phase-correct hardware pwm
    // this is better for motors as it ensures an even waveform
    // note, however, that fast pwm mode can achieve a frequency of up
    // 8 MHz (with a 16 MHz clock) at 50% duty cycle
    TCCR1B = 0;

    sbi(TCCR1B, CS11); // set timer 1 prescale factor to 64
    sbi(TCCR1B, CS10);
    sbi(TCCR1A, WGM10); // put timer 1 in 8-bit phase correct pwm mode
    sbi(TCCR3B, CS31);  // set timer 3 prescale factor to 64
    sbi(TCCR3B, CS30);
    sbi(TCCR3A, WGM30); // put timer 3 in 8-bit phase correct pwm mode
    sbi(TCCR4B, CS42);  // set timer4 prescale factor to 64
    sbi(TCCR4B, CS41);
    sbi(TCCR4B, CS40);
    sbi(TCCR4D, WGM40); // put timer 4 in phase- and frequency-correct PWM mode
    sbi(TCCR4A, PWM4A); // enable PWM mode for comparator OCR4A
    sbi(TCCR4C, PWM4D); // enable PWM mode for comparator OCR4D
    sbi(ADCSRA, ADPS2); // set a2d prescaler so we are inside the desired 50-200 KHz range.
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADEN);  // enable a2d conversions

    DDRB  |= (1 << 6);
    PORTB |= (1 << 6);
    DDRB  |= (1 << 7);
    PORTB |= (1 << 7);
    DDRB  |= (1 << 5);
    PORTB |= (1 << 5);

    power_timer2_disable();
    power_adc_disable();
    power_twi_disable();
    power_usart0_disable();
    power_usart1_disable();

    ab_key_init();
    ab_spi_init();
    ab_oled_init();
    ab_sound_init();

    ab_usb_init();
}
