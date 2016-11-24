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

#include "USBInterrupt.h"

void USB_INT_DisableAllInterrupts(void) {
    USBCON &= ~(1 << VBUSTE);
    UDIEN   = 0;
}

void USB_INT_ClearAllInterrupts(void) {
    USBINT = 0;
    UDINT  = 0;
}

ISR(USB_GEN_vect, ISR_BLOCK) {
    if (USB_INT_HasOccurred(USB_INT_VBUSTI) && USB_INT_IsEnabled(USB_INT_VBUSTI)) {
        USB_INT_Clear(USB_INT_VBUSTI);

        if (USB_VBUS_GetStatus()) {
            if (!(USB_Options & USB_OPT_MANUAL_PLL)) {
                USB_PLL_On();
                while (!(USB_PLL_IsReady()));
            }

            USB_DeviceState = DEVICE_STATE_Powered;
            EVENT_USB_Device_Connect();
        } else {
            if (!(USB_Options & USB_OPT_MANUAL_PLL)) USB_PLL_Off();

            USB_DeviceState = DEVICE_STATE_Unattached;
            EVENT_USB_Device_Disconnect();
        }
    }

    if (USB_INT_HasOccurred(USB_INT_SUSPI) && USB_INT_IsEnabled(USB_INT_SUSPI)) {
        USB_INT_Disable(USB_INT_SUSPI);
        USB_INT_Enable(USB_INT_WAKEUPI);

        USB_CLK_Freeze();

        if (!(USB_Options & USB_OPT_MANUAL_PLL)) USB_PLL_Off();

        USB_DeviceState = DEVICE_STATE_Suspended;
        EVENT_USB_Device_Suspend();
    }

    if (USB_INT_HasOccurred(USB_INT_WAKEUPI) && USB_INT_IsEnabled(USB_INT_WAKEUPI)) {
        if (!(USB_Options & USB_OPT_MANUAL_PLL)) {
            USB_PLL_On();
            while (!(USB_PLL_IsReady()));
        }

        USB_CLK_Unfreeze();

        USB_INT_Clear(USB_INT_WAKEUPI);

        USB_INT_Disable(USB_INT_WAKEUPI);
        USB_INT_Enable(USB_INT_SUSPI);

        if (USB_Device_ConfigurationNumber) {
            USB_DeviceState = DEVICE_STATE_Configured;
        } else {
            USB_DeviceState = (USB_Device_IsAddressSet()) ? DEVICE_STATE_Configured : DEVICE_STATE_Powered;
        }

        EVENT_USB_Device_WakeUp();
    }

    if (USB_INT_HasOccurred(USB_INT_EORSTI) && USB_INT_IsEnabled(USB_INT_EORSTI)) {
        USB_INT_Clear(USB_INT_EORSTI);

        USB_DeviceState                = DEVICE_STATE_Default;
        USB_Device_ConfigurationNumber = 0;

        USB_INT_Clear(USB_INT_SUSPI);
        USB_INT_Disable(USB_INT_SUSPI);
        USB_INT_Enable(USB_INT_WAKEUPI);

        Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL,
                                   ENDPOINT_DIR_OUT, USB_Device_ControlEndpointSize,
                                   ENDPOINT_BANK_SINGLE);

        EVENT_USB_Device_Reset();
    }
}
