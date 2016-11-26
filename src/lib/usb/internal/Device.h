/**
 *  Common USB Device definitions for all architectures.
 */

#pragma once

#include "../../ab_common.h"
#include "usb.h"
#include "Descriptors.h"
#include "USBInterrupt.h"
#include "Endpoint.h"
#include "Events.h"

#define CONTROL_REQTYPE_DIRECTION 0x80
#define CONTROL_REQTYPE_TYPE      0x60
#define CONTROL_REQTYPE_RECIPIENT 0x1F

#define REQDIR_HOSTTODEVICE (0 << 7)
#define REQDIR_DEVICETOHOST (1 << 7)

#define REQTYPE_STANDARD (0 << 5)
#define REQTYPE_CLASS    (1 << 5)

#define REQREC_DEVICE    (0 << 0)
#define REQREC_INTERFACE (1 << 0)
#define REQREC_ENDPOINT  (2 << 0)
#define REQREC_OTHER     (3 << 0)

#define REQ_GetStatus        0
#define REQ_ClearFeature     1
#define REQ_SetFeature       3
#define REQ_SetAddress       5
#define REQ_GetDescriptor    6
#define REQ_GetConfiguration 8
#define REQ_SetConfiguration 9

#define FEATURE_SEL_EndpointHalt 0x00

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

void USB_Device_ProcessControlRequest(void);

static INLINE bool USB_Device_IsAddressSet(void) {
    return (UDADDR & (1 << ADDEN));
}

static INLINE void USB_PLL_On(void) {
    PLLCSR = (1 << PINDIV);
    PLLCSR = ((1 << PINDIV) | (1 << PLLE));
}

static INLINE void USB_PLL_Off(void) {
    PLLCSR = 0;
}

static INLINE bool USB_PLL_IsReady(void) {
    return ((PLLCSR & (1 << PLOCK)) ? true : false);
}

static INLINE void USB_CLK_Freeze(void) {
    USBCON  |=  (1 << FRZCLK);
}

static INLINE void USB_CLK_Unfreeze(void) {
    USBCON  &= ~(1 << FRZCLK);
}