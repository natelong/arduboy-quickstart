#pragma once

#include <avr/pgmspace.h>

// Map SPI port to 'new' pins D14..D17
#define PIN_SPI_SS   (17)
#define PIN_SPI_MOSI (16)
#define PIN_SPI_MISO (14)
#define PIN_SPI_SCK  (15)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

// Mapping of analog pins as digital I/O
// A6-A11 share with digital pins
#define PIN_A0   (18)
#define PIN_A1   (19)
#define PIN_A2   (20)
#define PIN_A3   (21)
#define PIN_A4   (22)
#define PIN_A5   (23)
#define PIN_A6   (24)
#define PIN_A7   (25)
#define PIN_A8   (26)
#define PIN_A9   (27)
#define PIN_A10  (28)
#define PIN_A11  (29)

static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;
static const uint8_t A5  = PIN_A5;
static const uint8_t A6  = PIN_A6;	// D4
static const uint8_t A7  = PIN_A7;	// D6
static const uint8_t A8  = PIN_A8;	// D8
static const uint8_t A9  = PIN_A9;	// D9
static const uint8_t A10 = PIN_A10;	// D10
static const uint8_t A11 = PIN_A11;	// D12

#ifdef ARDUINO_MAIN
// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA32U4 / ARDUINO LEONARDO
//
// D0				PD2					RXD1/INT2
// D1				PD3					TXD1/INT3
// D2				PD1		SDA			SDA/INT1
// D3#				PD0		PWM8/SCL	OC0B/SCL/INT0
// D4		A6		PD4					ADC8
// D5#				PC6		???			OC3A/#OC4A
// D6#		A7		PD7		FastPWM		#OC4D/ADC10
// D7				PE6					INT6/AIN0
//
// D8		A8		PB4					ADC11/PCINT4
// D9#		A9		PB5		PWM16		OC1A/#OC4B/ADC12/PCINT5
// D10#		A10		PB6		PWM16		OC1B/0c4B/ADC13/PCINT6
// D11#				PB7		PWM8/16		0C0A/OC1C/#RTS/PCINT7
// D12		A11		PD6					T1/#OC4D/ADC9
// D13#				PC7		PWM10		CLK0/OC4A
//
// A0		D18		PF7					ADC7
// A1		D19		PF6					ADC6
// A2		D20 	PF5					ADC5
// A3		D21 	PF4					ADC4
// A4		D22		PF1					ADC1
// A5		D23 	PF0					ADC0
//
// New pins D14..D17 to map SPI port to digital pins
//
// MISO		D14		PB3					MISO,PCINT3
// SCK		D15		PB1					SCK,PCINT1
// MOSI		D16		PB2					MOSI,PCINT2
// SS		D17		PB0					RXLED,SS/PCINT0
//
// TXLED	D30		PD5					XCK1
// RXLED	D17	    PB0
// HWB				PE2					HWB

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
	(uint16_t) &DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
	(uint16_t) &PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
	(uint16_t) &PINF,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // D0 - PD2
	PD,	// D1 - PD3
	PD, // D2 - PD1
	PD,	// D3 - PD0
	PD,	// D4 - PD4
	PC, // D5 - PC6
	PD, // D6 - PD7
	PE, // D7 - PE6

	PB, // D8 - PB4
	PB,	// D9 - PB5
	PB, // D10 - PB6
	PB,	// D11 - PB7
	PD, // D12 - PD6
	PC, // D13 - PC7

	PB,	// D14 - MISO - PB3
	PB,	// D15 - SCK - PB1
	PB,	// D16 - MOSI - PB2
	PB,	// D17 - SS - PB0

	PF,	// D18 - A0 - PF7
	PF, // D19 - A1 - PF6
	PF, // D20 - A2 - PF5
	PF, // D21 - A3 - PF4
	PF, // D22 - A4 - PF1
	PF, // D23 - A5 - PF0

	PD, // D24 / D4 - A6 - PD4
	PD, // D25 / D6 - A7 - PD7
	PB, // D26 / D8 - A8 - PB4
	PB, // D27 / D9 - A9 - PB5
	PB, // D28 / D10 - A10 - PB6
	PD, // D29 / D12 - A11 - PD6
	PD, // D30 / TX Led - PD5
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(2), // D0 - PD2
	_BV(3),	// D1 - PD3
	_BV(1), // D2 - PD1
	_BV(0),	// D3 - PD0
	_BV(4),	// D4 - PD4
	_BV(6), // D5 - PC6
	_BV(7), // D6 - PD7
	_BV(6), // D7 - PE6

	_BV(4), // D8 - PB4
	_BV(5),	// D9 - PB5
	_BV(6), // D10 - PB6
	_BV(7),	// D11 - PB7
	_BV(6), // D12 - PD6
	_BV(7), // D13 - PC7

	_BV(3),	// D14 - MISO - PB3
	_BV(1),	// D15 - SCK - PB1
	_BV(2),	// D16 - MOSI - PB2
	_BV(0),	// D17 - SS - PB0

	_BV(7),	// D18 - A0 - PF7
	_BV(6), // D19 - A1 - PF6
	_BV(5), // D20 - A2 - PF5
	_BV(4), // D21 - A3 - PF4
	_BV(1), // D22 - A4 - PF1
	_BV(0), // D23 - A5 - PF0

	_BV(4), // D24 / D4 - A6 - PD4
	_BV(7), // D25 / D6 - A7 - PD7
	_BV(4), // D26 / D8 - A8 - PB4
	_BV(5), // D27 / D9 - A9 - PB5
	_BV(6), // D28 / D10 - A10 - PB6
	_BV(6), // D29 / D12 - A11 - PD6
	_BV(5), // D30 / TX Led - PD5
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0B,		/* 3 */
	NOT_ON_TIMER,
	TIMER3A,		/* 5 */
	TIMER4D,		/* 6 */
	NOT_ON_TIMER,

	NOT_ON_TIMER,
	TIMER1A,		/* 9 */
	TIMER1B,		/* 10 */
	TIMER0A,		/* 11 */

	NOT_ON_TIMER,
	TIMER4A,		/* 13 */

	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,

	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif /* ARDUINO_MAIN */