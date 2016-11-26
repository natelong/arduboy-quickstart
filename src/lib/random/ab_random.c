#include "ab_random.h"

static uint8_t _seed = 1;

void ab_random_seed(uint8_t seed) {
    _seed = seed;
}

uint8_t ab_random(void) {
    _seed ^= _seed >> 7;
    _seed ^= _seed << 3;
    _seed ^= _seed >> 5;
    return _seed * 57;
}