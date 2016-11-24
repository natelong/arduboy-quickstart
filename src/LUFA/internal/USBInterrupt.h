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
 *  USB controller interrupt service routine management.
 */

#pragma once

#include "Common.h"
#include "USBMode.h"
#include "Events.h"
#include "USBController.h"

enum USB_Interrupts_t {
    USB_INT_VBUSTI  = 0,
    USB_INT_WAKEUPI = 2,
    USB_INT_SUSPI   = 3,
    USB_INT_EORSTI  = 4,
    USB_INT_SOFI    = 5,
    USB_INT_RXSTPI  = 6,
};

static ALWAYS_INLINE void USB_INT_Enable(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  USBCON |= (1 << VBUSTE);  break;
        case USB_INT_WAKEUPI: UDIEN  |= (1 << WAKEUPE); break;
        case USB_INT_SUSPI:   UDIEN  |= (1 << SUSPE);   break;
        case USB_INT_EORSTI:  UDIEN  |= (1 << EORSTE);  break;
        case USB_INT_SOFI:    UDIEN  |= (1 << SOFE);    break;
        case USB_INT_RXSTPI:  UEIENX |= (1 << RXSTPE);  break;
    }
}

static ALWAYS_INLINE void USB_INT_Disable(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  USBCON &= ~(1 << VBUSTE);  break;
        case USB_INT_WAKEUPI: UDIEN  &= ~(1 << WAKEUPE); break;
        case USB_INT_SUSPI:   UDIEN  &= ~(1 << SUSPE);   break;
        case USB_INT_EORSTI:  UDIEN  &= ~(1 << EORSTE);  break;
        case USB_INT_SOFI:    UDIEN  &= ~(1 << SOFE);    break;
        case USB_INT_RXSTPI:  UEIENX &= ~(1 << RXSTPE);  break;
    }
}

static ALWAYS_INLINE void USB_INT_Clear(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  USBINT &= ~(1 << VBUSTI);  break;
        case USB_INT_WAKEUPI: UDINT  &= ~(1 << WAKEUPI); break;
        case USB_INT_SUSPI:   UDINT  &= ~(1 << SUSPI);   break;
        case USB_INT_EORSTI:  UDINT  &= ~(1 << EORSTI);  break;
        case USB_INT_SOFI:    UDINT  &= ~(1 << SOFI);    break;
        case USB_INT_RXSTPI:  UEINTX &= ~(1 << RXSTPI);  break;
    }
}

static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool USB_INT_IsEnabled(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  return (USBCON & (1 << VBUSTE));
        case USB_INT_WAKEUPI: return (UDIEN  & (1 << WAKEUPE));
        case USB_INT_SUSPI:   return (UDIEN  & (1 << SUSPE));
        case USB_INT_EORSTI:  return (UDIEN  & (1 << EORSTE));
        case USB_INT_SOFI:    return (UDIEN  & (1 << SOFE));
        case USB_INT_RXSTPI:  return (UEIENX & (1 << RXSTPE));
    }

    return false;
}

static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool USB_INT_HasOccurred(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  return (USBINT & (1 << VBUSTI));
        case USB_INT_WAKEUPI: return (UDINT  & (1 << WAKEUPI));
        case USB_INT_SUSPI:   return (UDINT  & (1 << SUSPI));
        case USB_INT_EORSTI:  return (UDINT  & (1 << EORSTI));
        case USB_INT_SOFI:    return (UDINT  & (1 << SOFI));
        case USB_INT_RXSTPI:  return (UEINTX & (1 << RXSTPI));
    }

    return false;
}

void USB_INT_ClearAllInterrupts(void);
void USB_INT_DisableAllInterrupts(void);
