// nate
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

/**
 *  Common USB Device definitions for all architectures.
 */

#pragma once

#include "../../ab_common.h"
#include "USBController.h"
#include "StdDescriptors.h"
#include "USBInterrupt.h"
#include "Endpoint.h"

/**
 * Enum for the various states of the USB Device state machine. Only some states are
 * implemented in the LUFA library - other states are left to the user to implement.
 */
enum USB_Device_States_t {
    // Internally implemented
    DEVICE_STATE_Unattached = 0, // Device is not currently connected to a host.
    DEVICE_STATE_Powered    = 1, // Device is connected to a host, but enumeration has not yet begun.
    DEVICE_STATE_Default    = 2, // Device's USB bus has been reset by host and waiting for host to begin the enumeration.
    DEVICE_STATE_Addressed  = 3, // Device has been addressed by the USB Host, but is not yet configured.
    // May be implemented by user
    DEVICE_STATE_Configured = 4, // Device enumerated by host and ready for USB communications to begin.
    DEVICE_STATE_Suspended  = 5, // USB bus suspended by host, and should power down to a minimal level until bus is resumed.
};

/** Function to retrieve a given descriptor's size and memory location from the given descriptor type value,
 *  index and language ID. This function MUST be overridden in the user application (added with full, identical
 *  prototype and name so that the library can call it to retrieve descriptor data.
 *
 *  \param[in] wValue               The type of the descriptor to retrieve in the upper byte, and the index in the
 *                                  lower byte (when more than one descriptor of the given type exists, such as the
 *                                  case of string descriptors). The type may be one of the standard types defined
 *                                  in the DescriptorTypes_t enum, or may be a class-specific descriptor type value.
 *  \param[out] DescriptorAddress   Pointer to the descriptor in memory. This should be set by the routine to
 *                                  the address of the descriptor.
 *  \param[out] MemoryAddressSpace  A value from the \ref USB_DescriptorMemorySpaces_t enum to indicate the memory
 *                                  space in which the descriptor is stored. This parameter does not exist when one
 *                                  of the \c USE_*_DESCRIPTORS compile time options is used, or on architectures which
 *                                  use a unified address space.
 *
 *  \return Size in bytes of the descriptor if it exists, zero or \ref NO_DESCRIPTOR otherwise.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const void** const DescriptorAddress);

static INLINE bool USB_Device_IsAddressSet(void) {
    return (UDADDR & (1 << ADDEN));
}