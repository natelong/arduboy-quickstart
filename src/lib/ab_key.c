#include "ab.h"

static uint8_t current = 0;
static uint8_t last    = 0;

void ab_key_init(void) {
    uint8_t oldSREG = SREG;
    cli();

    // L
    DDRF  &= ~(1 << 5);
    PORTF |=  (1 << 5);
    // R
    DDRF  &= ~(1 << 6);
    PORTF |=  (1 << 6);
    // U
    DDRF  &= ~(1 << 7);
    PORTF |=  (1 << 7);
    // D
    DDRF  &= ~(1 << 4);
    PORTF |=  (1 << 4);

    // A
    DDRE  &= ~(1 << 6);
    PORTE |=  (1 << 6);
    // B
    DDRB  &= ~(1 << 4);
    PORTB |=  (1 << 4);

    SREG = oldSREG;
}

void ab_key_update(void) {
    last     = current;
    current  = ((~PINF) & 0xf0);      // up, down, left, right
    current |= ((~PINE) & 0x40) >> 3; // A
    current |= ((~PINB) & 0x10) >> 2; // B
}

uint8_t ab_key_getCurrent(void) {
    return current;
}

uint8_t ab_key_getPressed(void) {
    return (last ^ current) & ~last;
}

uint8_t ab_key_getReleased(void) {
    return (last ^ current) & ~current;
}
