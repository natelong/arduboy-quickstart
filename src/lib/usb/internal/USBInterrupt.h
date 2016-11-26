/**
 *  USB controller interrupt service routine management.
 */

#pragma once

#include "../../ab_common.h"
#include "Events.h"
#include "usb.h"

enum USB_Interrupts_t {
    USB_INT_VBUSTI  = 0,
    USB_INT_WAKEUPI = 2,
    USB_INT_SUSPI   = 3,
    USB_INT_EORSTI  = 4,
    USB_INT_SOFI    = 5,
    USB_INT_RXSTPI  = 6,
};


static INLINE void USB_INT_EnableVBUS(void) {
    USBCON |= (1 << VBUSTE);
}

static INLINE void USB_INT_EnableWakeup(void) {
    UDIEN |= (1 << WAKEUPE);
}

static INLINE void USB_INT_EnableSuspend(void) {
    UDIEN |= (1 << SUSPE);
}

static INLINE void USB_INT_EnableReset(void) {
    UDIEN |= (1 << EORSTE);
}

static INLINE void USB_INT_Disable(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  USBCON &= ~(1 << VBUSTE);  break;
        case USB_INT_WAKEUPI: UDIEN  &= ~(1 << WAKEUPE); break;
        case USB_INT_SUSPI:   UDIEN  &= ~(1 << SUSPE);   break;
        case USB_INT_EORSTI:  UDIEN  &= ~(1 << EORSTE);  break;
        case USB_INT_SOFI:    UDIEN  &= ~(1 << SOFE);    break;
        case USB_INT_RXSTPI:  UEIENX &= ~(1 << RXSTPE);  break;
    }
}

static INLINE void USB_INT_Clear(const uint8_t Interrupt) {
    switch (Interrupt) {
        case USB_INT_VBUSTI:  USBINT &= ~(1 << VBUSTI);  break;
        case USB_INT_WAKEUPI: UDINT  &= ~(1 << WAKEUPI); break;
        case USB_INT_SUSPI:   UDINT  &= ~(1 << SUSPI);   break;
        case USB_INT_EORSTI:  UDINT  &= ~(1 << EORSTI);  break;
        case USB_INT_SOFI:    UDINT  &= ~(1 << SOFI);    break;
        case USB_INT_RXSTPI:  UEINTX &= ~(1 << RXSTPI);  break;
    }
}

static INLINE bool USB_INT_IsEnabled(const uint8_t Interrupt) {
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

static INLINE bool USB_INT_HasOccurred(const uint8_t Interrupt) {
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
