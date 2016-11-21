// nate
#pragma once

/** Forces GCC to create a memory barrier, ensuring that memory accesses are not reordered past the barrier point.
 *  This can be used before ordering-critical operations, to ensure that the compiler does not re-order the resulting
 *  assembly output in an unexpected manner on sections of code that are ordering-specific.
 */
#define GCC_MEMORY_BARRIER() __asm__ __volatile__("" ::: "memory");

/** Evaluates to boolean true if the specified value can be determined at compile time to be a constant value
 *  when compiling under GCC.
 *
 *  \param[in] x  Value to check compile time constantness of.
 *
 *  \return Boolean true if the given value is known to be a compile time constant, false otherwise.
 */
#define GCC_IS_COMPILE_CONST(x) __builtin_constant_p(x)

/** Indicates that the function returns a value which should not be ignored by the user code. When
 *  applied, any ignored return value from calling the function will produce a compiler warning.
 */
#define ATTR_WARN_UNUSED_RESULT     __attribute__ ((warn_unused_result))

/** Indicates that the specified parameters of the function are pointers which should never be \c NULL.
 *  When applied as a 1-based comma separated list the compiler will emit a warning if the specified
 *  parameters are known at compiler time to be \c NULL at the point of calling the function.
 */
#define ATTR_NON_NULL_PTR_ARG(...)  __attribute__ ((nonnull (__VA_ARGS__)))

/** Forces the compiler to inline the specified function. When applied, the given function will be
 *  in-lined under all circumstances.
 */
#define ATTR_ALWAYS_INLINE          __attribute__ ((always_inline))

/** Indicates that the specified function is constant, in that it has no side effects other than
 *  parameter access.
 */
#define ATTR_CONST                  __attribute__ ((const))

/** Marks a function as a weak reference, which can be overridden by other functions with an
 *  identical name (in which case the weak reference is discarded at link time).
 */
#define ATTR_WEAK                   __attribute__ ((weak))

/** Marks a function as an alias for another function.
 *
 *  \param[in] Func  Name of the function which the given function name should alias.
 */
#define ATTR_ALIAS(Func)               __attribute__ ((alias( #Func )))

/** Marks a variable or struct element for packing into the smallest space available, omitting any
 *  alignment bytes usually added between fields to optimize field accesses.
 */
#define ATTR_PACKED                     __attribute__ ((packed))

/** Macro for encasing other multi-statement macros. This should be used along with an opening brace
 *  before the start of any multi-statement macro, so that the macros contents as a whole are treated
 *  as a discrete block and not as a list of separate statements which may cause problems when used as
 *  a block (such as inline \c if statements).
 */
#define MACROS do

/** Macro for encasing other multi-statement macros. This should be used along with a preceding closing
 *  brace at the end of any multi-statement macro, so that the macros contents as a whole are treated
 *  as a discrete block and not as a list of separate statements which may cause problems when used as
 *  a block (such as inline \c if statements).
 */
#define MACROE while (0)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))