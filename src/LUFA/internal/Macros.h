// nate
#pragma once

// Forces the compiler to inline the specified function under all circumstances.
#define ALWAYS_INLINE inline __attribute__ ((always_inline))

/** Marks a variable or struct element for packing into the smallest space available, omitting any
 *  alignment bytes usually added between fields to optimize field accesses.
 */
#define ATTR_PACKED __attribute__ ((packed))