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

#include "USBController.h"

void USB_Init() {
    USB_REG_On();
    PLLFRQ = ((1 << PLLUSB) | (1 << PDIV3) | (1 << PDIV1));
    USB_IsInitialized = true;
    USB_ResetInterface();
}

void USB_Disable(void) {
    USB_INT_DisableAllInterrupts();
    USB_INT_ClearAllInterrupts();
    USB_Detach();
    USB_Controller_Disable();
    USB_PLL_Off();
    USB_REG_Off();
    USB_OTGPAD_Off();
    USB_IsInitialized = false;
}

void USB_ResetInterface(void) {
    USB_INT_DisableAllInterrupts();
    USB_INT_ClearAllInterrupts();
    USB_Controller_Reset();
    USB_CLK_Unfreeze();
    USB_PLL_Off();

    // USB_Init_Device
    USB_DeviceState = DEVICE_STATE_Unattached;
    USB_Device_ConfigurationNumber = 0;
    USB_Device_SetFullSpeed();
    USB_INT_Enable(USB_INT_VBUSTI);
    Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL, ENDPOINT_DIR_OUT, USB_Device_ControlEndpointSize, ENDPOINT_BANK_SINGLE);
    USB_INT_Clear(USB_INT_SUSPI);
    USB_INT_Enable(USB_INT_SUSPI);
    USB_INT_Enable(USB_INT_EORSTI);
    USB_Attach();

    USB_OTGPAD_On();
}
