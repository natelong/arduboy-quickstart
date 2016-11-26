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
 * Common USB Controller definitions for all architectures.
 */

#pragma once

#include "../../ab_common.h"

/**
 * Endpoint address direction masks for a Host to Device endpoint.
 * These may be ORed with the index of the address within a device to obtain the full endpoint address.
 */
#define ENDPOINT_DIR_OUT 0x00 // Mask for OUT direction
#define ENDPOINT_DIR_IN  0x80 // Mask for IN direction

#define EP_TYPE_CONTROL     0x00 // Mask for a CONTROL type endpoint or pipe.
#define EP_TYPE_ISOCHRONOUS 0x01 // Mask for an ISOCHRONOUS type endpoint or pipe.
#define EP_TYPE_BULK        0x02 // Mask for a BULK type endpoint or pipe.
#define EP_TYPE_INTERRUPT   0x03 // Mask for an INTERRUPT type endpoint or pipe.

#ifndef F_USB
    #error F_USB is not defined. You must define F_USB to the frequency of the unprescaled USB controller clock in your project makefile.
#endif

#include "Events.h"
#include "USBTask.h"
#include "USBInterrupt.h"

#include "Device.h"
#include "Endpoint.h"
#include "DeviceStandardReq.h"
#include "EndpointStream.h"

#define USB_PLL_PSC (1 << PINDIV)

/** Constant for the maximum software timeout period of the USB data stream transfer functions
 *  (both control and standard) when in either device or host mode. If the next packet of a stream
 *  is not received or acknowledged within this time period, the stream function will fail.
 *
 *  This value may be overridden in the user project makefile as the value of the
 *  \ref USB_STREAM_TIMEOUT_MS token, and passed to the compiler using the -D switch.
 */
#define USB_STREAM_TIMEOUT_MS 100

/** Determines if the VBUS line is currently high (i.e. the USB host is supplying power).
 *
 *  \note This function is not available on some AVR models which do not support hardware VBUS monitoring.
 *
 *  \return Boolean \c true if the VBUS line is currently detecting power from a host, \c false otherwise.
 */
static INLINE bool USB_VBUS_GetStatus(void) {
    return ((USBSTA & (1 << VBUS)) ? true : false);
}

/** Main function to initialize and start the USB interface. Once active, the USB interface will
 *  allow for device connection to a host when in device mode, or for device enumeration while in
 *  host mode.
 *
 *  As the USB library relies on interrupts for the device and host mode enumeration processes,
 *  the user must enable global interrupts before or shortly after this function is called. In
 *  device mode, interrupts must be enabled within 500ms of this function being called to ensure
 *  that the host does not time out whilst enumerating the device. In host mode, interrupts may be
 *  enabled at the application's leisure however enumeration will not begin of an attached device
 *  until after this has occurred.
 *
 *  Calling this function when the USB interface is already initialized will cause a complete USB
 *  interface reset and re-enumeration.
 *
 *  \note The mode parameter does not exist on devices where only one mode is possible, such as USB
 *        AVR models which only implement the USB device mode in hardware.
 */
void USB_Init();

/** Shuts down the USB interface. This turns off the USB interface after deallocating all USB FIFO
 *  memory, endpoints and pipes. When turned off, no USB functionality can be used until the interface
 *  is restarted with the \ref USB_Init() function.
 */
void USB_Disable(void);

static INLINE void USB_PLL_On(void) {
    PLLCSR = USB_PLL_PSC;
    PLLCSR = (USB_PLL_PSC | (1 << PLLE));
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
