#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ARD_MINI

#define HIGH 0x1
#define LOW  0x0

#define INPUT        0x0
#define OUTPUT       0x1
#define INPUT_PULLUP 0x2

#define mini_clocksPerMicrosecond() ( F_CPU / 1000000L )
#define mini_clocksToMicroseconds(a) ( (a) / mini_clocksPerMicrosecond() )

#define mini_bitSet(value, bit) ((value) |= (1UL << (bit)))

void mini_init(void);
void mini_pinMode(uint8_t, uint8_t);
void mini_digitalWrite(uint8_t, uint8_t);

uint32_t mini_millis(void);
uint32_t mini_micros(void);
void     mini_delay(uint32_t);

// On the ATmega1280, the addresses of some of the port registers are
// greater than 255, so we can't store them in uint8_t's.
extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
//
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P)    ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )

#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P)  ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P)   ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#define NOT_A_PIN         0
#define NOT_A_PORT        0
#define NOT_AN_INTERRUPT -1

typedef struct {
    volatile uint8_t* mode;
    volatile uint8_t* out;
    volatile uint8_t* in;
             uint8_t  mask;
} mini_Pin;

void mini_pinModeInput(const mini_Pin* pin);
void mini_pinModeInputPullup(const mini_Pin* pin);
void mini_pinModeOutput(const mini_Pin* pin);
void mini_setPinLow(const mini_Pin* pin);
void mini_setPinHigh(const mini_Pin* pin);

extern const mini_Pin mini_SPI_MISO;
extern const mini_Pin mini_SPI_SCK;
extern const mini_Pin mini_SPI_MOSI;
extern const mini_Pin mini_SPI_SS;

extern const mini_Pin mini_OLED_CS;
extern const mini_Pin mini_OLED_DC;
extern const mini_Pin mini_OLED_RST;



#ifdef ARDUINO_MAIN

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

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
    PD, // D1 - PD3
    PD, // D2 - PD1
    PD, // D3 - PD0
    PD, // D4 - PD4
    PC, // D5 - PC6
    PD, // D6 - PD7
    PE, // D7 - PE6

    PB, // D8 - PB4
    PB, // D9 - PB5
    PB, // D10 - PB6
    PB, // D11 - PB7
    PD, // D12 - PD6
    PC, // D13 - PC7

    PB, // D14 - MISO - PB3
    PB, // D15 - SCK - PB1
    PB, // D16 - MOSI - PB2
    PB, // D17 - SS - PB0

    PF, // D18 - A0 - PF7
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
    _BV(3), // D1 - PD3
    _BV(1), // D2 - PD1
    _BV(0), // D3 - PD0
    _BV(4), // D4 - PD4
    _BV(6), // D5 - PC6
    _BV(7), // D6 - PD7
    _BV(6), // D7 - PE6

    _BV(4), // D8 - PB4
    _BV(5), // D9 - PB5
    _BV(6), // D10 - PB6
    _BV(7), // D11 - PB7
    _BV(6), // D12 - PD6
    _BV(7), // D13 - PC7

    _BV(3), // D14 - MISO - PB3
    _BV(1), // D15 - SCK - PB1
    _BV(2), // D16 - MOSI - PB2
    _BV(0), // D17 - SS - PB0

    _BV(7), // D18 - A0 - PF7
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

#endif /* ARDUINO_MAIN */

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