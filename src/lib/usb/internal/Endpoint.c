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

#include "Endpoint.h"

void Endpoint_ClearEndpoints(void) {
    UEINT = 0;

    for (uint8_t EPNum = 0; EPNum < ENDPOINT_TOTAL_ENDPOINTS; EPNum++) {
        Endpoint_SelectEndpoint(EPNum);
        UEIENX  = 0;
        UEINTX  = 0;
        UECFG1X = 0;
        Endpoint_DisableEndpoint();
    }
}

void Endpoint_ClearStatusStage(void) {
    if (USB_ControlRequest.bmRequestType & REQDIR_DEVICETOHOST) {
        while (!(Endpoint_IsOUTReceived())) {
            if (USB_DeviceState == DEVICE_STATE_Unattached) return;
        }
        Endpoint_ClearOUT();
    } else {
        while (!(Endpoint_IsINReady())) {
            if (USB_DeviceState == DEVICE_STATE_Unattached) return;
        }
        Endpoint_ClearIN();
    }
}

uint8_t Endpoint_WaitUntilReady(void) {
    uint8_t  TimeoutMSRem = USB_STREAM_TIMEOUT_MS;
    uint16_t PreviousFrameNumber = UDFNUM;

    for (;;) {
        if (Endpoint_GetEndpointDirection() == ENDPOINT_DIR_IN) {
            if (Endpoint_IsINReady()) return ENDPOINT_READYWAIT_NoError;
        } else {
            if (Endpoint_IsOUTReceived()) return ENDPOINT_READYWAIT_NoError;
        }

        uint8_t USB_DeviceState_LCL = USB_DeviceState;

        if (USB_DeviceState_LCL == DEVICE_STATE_Unattached) {
            return ENDPOINT_READYWAIT_DeviceDisconnected;
        } else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended) {
            return ENDPOINT_READYWAIT_BusSuspended;
        } else if (Endpoint_IsStalled()) {
            return ENDPOINT_READYWAIT_EndpointStalled;
        }

        uint16_t CurrentFrameNumber = UDFNUM;

        if (CurrentFrameNumber != PreviousFrameNumber) {
            PreviousFrameNumber = CurrentFrameNumber;
            if (!(TimeoutMSRem--)) return ENDPOINT_READYWAIT_Timeout;
        }
    }
}
