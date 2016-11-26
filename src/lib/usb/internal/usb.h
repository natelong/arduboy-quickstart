/**
 * Common USB Controller definitions for all architectures.
 */

#pragma once

#include "../../ab_common.h"

#ifndef F_USB
    #error F_USB is not defined. You must define F_USB to the frequency of the unprescaled USB controller clock in your project makefile.
#endif

// #include "Events.h"
// #include "USBInterrupt.h"

#include "Device.h"
// #include "Endpoint.h"
#include "DeviceStandardReq.h"
// #include "EndpointStream.h"

extern USB_Request_Header_t USB_ControlRequest;
#define USB_DeviceState GPIOR0

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
