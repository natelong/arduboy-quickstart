/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
*/

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"

void mini_pinMode(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg, *out;

	if (port == NOT_A_PIN) return;

	reg = portModeRegister(port);
	out = portOutputRegister(port);

	if (mode == INPUT) {
		uint8_t oldSREG = SREG;
        cli();
		*reg &= ~bit;
		*out &= ~bit;
		SREG = oldSREG;
	} else if (mode == INPUT_PULLUP) {
		uint8_t oldSREG = SREG;
        cli();
		*reg &= ~bit;
		*out |= bit;
		SREG = oldSREG;
	} else {
		uint8_t oldSREG = SREG;
        cli();
		*reg |= bit;
		SREG = oldSREG;
	}
}

static void turnOffPWM(uint8_t timer) {
	switch (timer) {
		case TIMER1A: cbi(TCCR1A, COM1A1); break;
		case TIMER1B: cbi(TCCR1A, COM1B1); break;
		case TIMER1C: cbi(TCCR1A, COM1C1); break;
		case TIMER0A: cbi(TCCR0A, COM0A1); break;
		case TIMER0B: cbi(TCCR0A, COM0B1); break;
		case TIMER3A: cbi(TCCR3A, COM3A1); break;
		case TIMER3B: cbi(TCCR3A, COM3B1); break;
		case TIMER3C: cbi(TCCR3A, COM3C1); break;
		case TIMER4A: cbi(TCCR4A, COM4A1); break;
		case TIMER4B: cbi(TCCR4A, COM4B1); break;
		case TIMER4D: cbi(TCCR4C, COM4D1); break;
	}
}

void mini_digitalWrite(uint8_t pin, uint8_t val) {
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;

	if (port == NOT_A_PIN) return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	out = portOutputRegister(port);

	uint8_t oldSREG = SREG;
	cli();

	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}

	SREG = oldSREG;
}
