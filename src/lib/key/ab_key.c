#include "../ab.h"

static uint8_t current = 0;
static uint8_t last    = 0;

void ab_key_init(void) {
    uint8_t oldSREG = SREG;
    cli();

    DDRF  &= ~(1 << 5); // L
    PORTF |=  (1 << 5);
    DDRF  &= ~(1 << 6); // R
    PORTF |=  (1 << 6);
    DDRF  &= ~(1 << 7); // U
    PORTF |=  (1 << 7);
    DDRF  &= ~(1 << 4); // D
    PORTF |=  (1 << 4);
    DDRE  &= ~(1 << 6); // A
    PORTE |=  (1 << 6);
    DDRB  &= ~(1 << 4); // B
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
