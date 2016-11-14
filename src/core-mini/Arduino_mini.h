#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ARD_MINI

typedef struct {
    volatile uint8_t* mode;
    volatile uint8_t* out;
    // volatile uint8_t* in;
             uint8_t  mask;
} mini_Pin;

#define  mini_clocksPerMicrosecond() ( F_CPU / 1000000L )
#define  mini_clocksToMicroseconds(a) ( (a) / mini_clocksPerMicrosecond() )

void     mini_init(void);
uint32_t mini_millis(void);
uint32_t mini_micros(void);
void     mini_delay(uint32_t);
#define  mini_bitSet(value, bit) ((value) |= (1UL << (bit)))

void     mini_pinModeInput(const mini_Pin* pin);
void     mini_pinModeInputPullup(const mini_Pin* pin);
void     mini_pinModeOutput(const mini_Pin* pin);
void     mini_setPinLow(const mini_Pin* pin);
void     mini_setPinHigh(const mini_Pin* pin);

extern const mini_Pin mini_SPI_MISO;
extern const mini_Pin mini_SPI_SCK;
extern const mini_Pin mini_SPI_MOSI;
extern const mini_Pin mini_SPI_SS;

extern const mini_Pin mini_OLED_CS;
extern const mini_Pin mini_OLED_DC;
extern const mini_Pin mini_OLED_RST;

extern const mini_Pin mini_SND_1;
extern const mini_Pin mini_SND_2;

extern const mini_Pin mini_KEY_L;
extern const mini_Pin mini_KEY_R;
extern const mini_Pin mini_KEY_U;
extern const mini_Pin mini_KEY_D;
extern const mini_Pin mini_KEY_A;
extern const mini_Pin mini_KEY_B;
