/**
 *  USB controller interrupt service routine management.
 */

#pragma once

#include "../ab_common.h"
#include "usb.h"

#define USB_INT_VBUSTI  0
#define USB_INT_WAKEUPI 2
#define USB_INT_SUSPI   3
#define USB_INT_EORSTI  4

static INLINE void USB_INT_EnableVBUS(void)    { USBCON |= (1 << VBUSTE);  }
static INLINE void USB_INT_EnableWakeup(void)  { UDIEN  |= (1 << WAKEUPE); }
static INLINE void USB_INT_EnableSuspend(void) { UDIEN  |= (1 << SUSPE);   }
static INLINE void USB_INT_EnableReset(void)   { UDIEN  |= (1 << EORSTE);  }

static INLINE void USB_INT_DisableVBUS(void)    { USBCON &= ~(1 << VBUSTE);  }
static INLINE void USB_INT_DisableWakeup(void)  { UDIEN  &= ~(1 << WAKEUPE); }
static INLINE void USB_INT_DisableSuspend(void) { UDIEN  &= ~(1 << SUSPE);   }
static INLINE void USB_INT_DisableReset(void)   { UDIEN  &= ~(1 << EORSTE);  }

static INLINE void USB_INT_ClearVBUS(void)    { USBINT &= ~(1 << VBUSTI);  }
static INLINE void USB_INT_ClearWakeup(void)  { UDINT  &= ~(1 << WAKEUPI); }
static INLINE void USB_INT_ClearSuspend(void) { UDINT  &= ~(1 << SUSPI);   }
static INLINE void USB_INT_ClearReset(void)   { UDINT  &= ~(1 << EORSTI);  }

static INLINE bool USB_INT_HasVBUSOccurred(void)    { return (USBINT & (1 << VBUSTI))  && (USBCON & (1 << VBUSTE));  }
static INLINE bool USB_INT_HasWakeupOccurred(void)  { return (UDINT  & (1 << WAKEUPI)) && (UDIEN  & (1 << WAKEUPE)); }
static INLINE bool USB_INT_HasSuspendOccurred(void) { return (UDINT  & (1 << SUSPI))   && (UDIEN  & (1 << SUSPE));   }
static INLINE bool USB_INT_HasResetOccurred(void)   { return (UDINT  & (1 << EORSTI))  && (UDIEN  & (1 << EORSTE));  }

void USB_INT_ClearAllInterrupts(void);
void USB_INT_DisableAllInterrupts(void);
