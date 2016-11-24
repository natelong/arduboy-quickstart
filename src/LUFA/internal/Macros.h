// nate
#pragma once

// Forces the compiler to inline the specified function under all circumstances.
#define ALWAYS_INLINE inline __attribute__ ((always_inline))

/** Marks a function as a weak reference, which can be overridden by other functions with an
 *  identical name (in which case the weak reference is discarded at link time).
 */
#define ATTR_WEAK __attribute__ ((weak))

/** Marks a function as an alias for another function.
 *
 *  \param[in] Func  Name of the function which the given function name should alias.
 */
#define ATTR_ALIAS(Func) __attribute__ ((alias( #Func )))

/** Marks a variable or struct element for packing into the smallest space available, omitting any
 *  alignment bytes usually added between fields to optimize field accesses.
 */
#define ATTR_PACKED __attribute__ ((packed))