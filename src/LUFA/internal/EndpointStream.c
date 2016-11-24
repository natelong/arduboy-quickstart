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

#define  __INCLUDE_FROM_USB_DRIVER
#include "USBMode.h"

#include "EndpointStream.h"

/* The following abuses the C preprocessor in order to copy-paste common code with slight alterations,
 * so that the code needs to be written once. It is a crude form of templating to reduce code maintenance. */

uint8_t Endpoint_Write_Stream_LE(const void* const Buffer, uint16_t Length, uint16_t* const BytesProcessed) {
    uint8_t* DataStream      = ((uint8_t*)Buffer);
    uint16_t BytesInTransfer = 0;
    uint8_t  ErrorCode;

    if ((ErrorCode = Endpoint_WaitUntilReady())) return ErrorCode;

    if (BytesProcessed != NULL) {
        Length -= *BytesProcessed;
        DataStream += *BytesProcessed;
    }

    while (Length) {
        if (!(Endpoint_IsReadWriteAllowed())) {
            Endpoint_ClearIN();

            #if !defined(INTERRUPT_CONTROL_ENDPOINT)
            USB_USBTask();
            #endif

            if (BytesProcessed != NULL) {
                *BytesProcessed += BytesInTransfer;
                return ENDPOINT_RWSTREAM_IncompleteTransfer;
            }

            if ((ErrorCode = Endpoint_WaitUntilReady())) return ErrorCode;
        } else {
            Endpoint_Write_8(*DataStream);
            DataStream += 1;
            Length--;
            BytesInTransfer++;
        }
    }

    return ENDPOINT_RWSTREAM_NoError;
}

uint8_t Endpoint_Write_Control_Stream_LE(const void* const Buffer, uint16_t Length) {
    uint8_t* DataStream     = ((uint8_t*)Buffer);
    bool     LastPacketFull = false;

    if (Length > USB_ControlRequest.wLength) {
        Length = USB_ControlRequest.wLength;
    } else if (!(Length)) {
        Endpoint_ClearIN();
    }

    while (Length || LastPacketFull) {
        uint8_t USB_DeviceState_LCL = USB_DeviceState;

        if (USB_DeviceState_LCL == DEVICE_STATE_Unattached) {
            return ENDPOINT_RWCSTREAM_DeviceDisconnected;
        } else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended) {
            return ENDPOINT_RWCSTREAM_BusSuspended;
        } else if (Endpoint_IsSETUPReceived()) {
            return ENDPOINT_RWCSTREAM_HostAborted;
        } else if (Endpoint_IsOUTReceived()) {
            break;
        }

        if (Endpoint_IsINReady()) {
            uint16_t BytesInEndpoint = Endpoint_BytesInEndpoint();

            while (Length && (BytesInEndpoint < USB_Device_ControlEndpointSize)) {
                Endpoint_Write_8(*DataStream);
                DataStream += 1;
                Length--;
                BytesInEndpoint++;
            }

            LastPacketFull = (BytesInEndpoint == USB_Device_ControlEndpointSize);
            Endpoint_ClearIN();
        }
    }

    while (!(Endpoint_IsOUTReceived())) {
        uint8_t USB_DeviceState_LCL = USB_DeviceState;

        if (USB_DeviceState_LCL == DEVICE_STATE_Unattached) {
            return ENDPOINT_RWCSTREAM_DeviceDisconnected;
        } else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended) {
            return ENDPOINT_RWCSTREAM_BusSuspended;
        }
    }

    return ENDPOINT_RWCSTREAM_NoError;
}

uint8_t Endpoint_Read_Control_Stream_LE(void* const Buffer, uint16_t Length) {
    uint8_t* DataStream = ((uint8_t*)Buffer);

    if (!(Length)) Endpoint_ClearOUT();

    while (Length) {
        uint8_t USB_DeviceState_LCL = USB_DeviceState;

        if (USB_DeviceState_LCL == DEVICE_STATE_Unattached) {
            return ENDPOINT_RWCSTREAM_DeviceDisconnected;
        } else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended) {
            return ENDPOINT_RWCSTREAM_BusSuspended;
        } else if (Endpoint_IsSETUPReceived()) {
            return ENDPOINT_RWCSTREAM_HostAborted;
        }

        if (Endpoint_IsOUTReceived()) {
            while (Length && Endpoint_BytesInEndpoint()) {
                *DataStream = Endpoint_Read_8();
                DataStream += 1;
                Length--;
            }

            Endpoint_ClearOUT();
        }
    }

    while (!(Endpoint_IsINReady())) {
        uint8_t USB_DeviceState_LCL = USB_DeviceState;

        if (USB_DeviceState_LCL == DEVICE_STATE_Unattached) {
            return ENDPOINT_RWCSTREAM_DeviceDisconnected;
        } else if (USB_DeviceState_LCL == DEVICE_STATE_Suspended) {
            return ENDPOINT_RWCSTREAM_BusSuspended;
        }
    }

    return ENDPOINT_RWCSTREAM_NoError;
}