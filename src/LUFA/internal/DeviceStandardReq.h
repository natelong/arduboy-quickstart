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
 *  USB device standard request management.
 */

#pragma once

#include "Common.h"
#include "StdDescriptors.h"
#include "Events.h"
#include "StdRequestType.h"
#include "USBTask.h"
#include "USBController.h"

/** Enum for the possible descriptor memory spaces, for the MemoryAddressSpace parameter of the
 *  CALLBACK_USB_GetDescriptor() function. This can be used when none of the USE_*_DESCRIPTORS
 *  compile time options are used, to indicate in which memory space the descriptor is stored.
 */
enum USB_DescriptorMemorySpaces_t {
    MEMSPACE_FLASH    = 0, // Indicates the requested descriptor is located in FLASH memory.
    MEMSPACE_EEPROM   = 1, // Indicates the requested descriptor is located in EEPROM memory.
    MEMSPACE_RAM      = 2, // Indicates the requested descriptor is located in RAM memory.
};

/** Indicates the currently set configuration number of the device. USB devices may have several
 *  different configurations which the host can select between; this indicates the currently selected
 *  value, or 0 if no configuration has been selected.
 */
extern uint8_t USB_Device_ConfigurationNumber;

/* Function Prototypes: */
void USB_Device_ProcessControlRequest(void);

#if defined(__INCLUDE_FROM_DEVICESTDREQ_C)
    static void USB_Device_SetAddress(void);
    static void USB_Device_SetConfiguration(void);
    static void USB_Device_GetConfiguration(void);
    static void USB_Device_GetDescriptor(void);
    static void USB_Device_GetStatus(void);
    static void USB_Device_ClearSetFeature(void);
#endif
