/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#pragma once

/* Includes: */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "Macros.h"
#include "Endianness.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/boot.h>
#include <util/delay.h>

typedef uint8_t uint_reg_t;

#define ARCH_AVR8 0
#define ARCH      ARCH_AVR8

#define ARCH_HAS_EEPROM_ADDRESS_SPACE
#define ARCH_HAS_FLASH_ADDRESS_SPACE
#define ARCH_HAS_MULTI_ADDRESS_SPACE

/** Function to perform a blocking delay for a specified number of milliseconds. The actual delay will be
 *  at a minimum the specified number of milliseconds, however due to loop overhead and internal calculations
 *  may be slightly higher.
 *
 *  \param[in] Milliseconds  Number of milliseconds to delay
 */
static inline void Delay_MS(uint16_t Milliseconds) ATTR_ALWAYS_INLINE;
static inline void Delay_MS(uint16_t Milliseconds)
{
	if (GCC_IS_COMPILE_CONST(Milliseconds)) {
		_delay_ms(Milliseconds);
	} else {
		while (Milliseconds--) {
			_delay_ms(1);
		}
	}
}

/** Retrieves a mask which contains the current state of the global interrupts for the device. This
 *  value can be stored before altering the global interrupt enable state, before restoring the
 *  flag(s) back to their previous values after a critical section using \ref SetGlobalInterruptMask().
 *
 *  \ingroup Group_GlobalInt
 *
 *  \return  Mask containing the current Global Interrupt Enable Mask bit(s).
 */
static inline uint_reg_t GetGlobalInterruptMask(void) ATTR_ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT;
static inline uint_reg_t GetGlobalInterruptMask(void)
{
	GCC_MEMORY_BARRIER();
	return SREG;
	GCC_MEMORY_BARRIER();
}

/** Sets the global interrupt enable state of the microcontroller to the mask passed into the function.
 *  This can be combined with \ref GetGlobalInterruptMask() to save and restore the Global Interrupt Enable
 *  Mask bit(s) of the device after a critical section has completed.
 *
 *  \ingroup Group_GlobalInt
 *
 *  \param[in] GlobalIntState  Global Interrupt Enable Mask value to use
 */
static inline void SetGlobalInterruptMask(const uint_reg_t GlobalIntState) ATTR_ALWAYS_INLINE;
static inline void SetGlobalInterruptMask(const uint_reg_t GlobalIntState)
{
	GCC_MEMORY_BARRIER();
	SREG = GlobalIntState;
	GCC_MEMORY_BARRIER();
}

/** Enables global interrupt handling for the device, allowing interrupts to be handled.
 *
 *  \ingroup Group_GlobalInt
 */
static inline void GlobalInterruptEnable(void) ATTR_ALWAYS_INLINE;
static inline void GlobalInterruptEnable(void)
{
	GCC_MEMORY_BARRIER();
	sei();
	GCC_MEMORY_BARRIER();
}

/** Disabled global interrupt handling for the device, preventing interrupts from being handled.
 *
 *  \ingroup Group_GlobalInt
 */
static inline void GlobalInterruptDisable(void) ATTR_ALWAYS_INLINE;
static inline void GlobalInterruptDisable(void)
{
	GCC_MEMORY_BARRIER();
	cli();
	GCC_MEMORY_BARRIER();
}
