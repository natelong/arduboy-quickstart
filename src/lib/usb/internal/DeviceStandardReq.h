/**
 *  USB device standard request management.
 */

#pragma once

#include "../../ab_common.h"
#include "StdDescriptors.h"
#include "Events.h"
#include "StdRequestType.h"
#include "usb.h"

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
