#include "wiring_private.h"

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (mini_clocksToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

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

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;

	cli();
	m = timer0_overflow_count;

	t = TCNT0;

	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;

	SREG = oldSREG;

	return ((m << 8) + t) * (64 / mini_clocksPerMicrosecond());
}

void delay(unsigned long ms)
{
	uint32_t start = micros();

	while (ms > 0) {
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

void mini_init() {
	// this needs to be called before setup() or some functions won't work there
	sei();

	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)
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
}
