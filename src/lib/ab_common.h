#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/power.h>

// Forces the compiler to inline the specified function under all circumstances.
#define INLINE inline __attribute__ ((always_inline))

// Marks a variable or struct element for packing into the smallest space available, omitting any
// alignment bytes usually added between fields to optimize field accesses.
#define PACKED __attribute__ ((packed))