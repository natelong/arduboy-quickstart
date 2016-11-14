#include "ab.h"

ab_KeyState key;

void ab_key_init(void) {
    memset(&key, 0x00, sizeof(ab_KeyState));

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
    key.last     = key.current;
    key.current  = ((~PINF) & 0xf0);      // up, down, left, right
    key.current |= ((~PINE) & 0x40) >> 3; // A
    key.current |= ((~PINB) & 0x10) >> 2; // B
}

uint8_t ab_key_getCurrent(void) {
    return key.current;
}

uint8_t ab_key_getPressed(void) {
    return (key.last ^ key.current) & ~key.last;
}

uint8_t ab_key_getReleased(void) {
    return (key.last ^ key.current) & ~key.current;
}
