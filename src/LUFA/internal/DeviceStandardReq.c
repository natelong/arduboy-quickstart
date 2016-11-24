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

#define  __INCLUDE_FROM_DEVICESTDREQ_C
#include "DeviceStandardReq.h"

uint8_t USB_Device_ConfigurationNumber;

void USB_Device_ProcessControlRequest(void)
{
    #if defined(ARCH_BIG_ENDIAN)
    USB_ControlRequest.bmRequestType = Endpoint_Read_8();
    USB_ControlRequest.bRequest      = Endpoint_Read_8();
    USB_ControlRequest.wValue        = Endpoint_Read_16_LE();
    USB_ControlRequest.wIndex        = Endpoint_Read_16_LE();
    USB_ControlRequest.wLength       = Endpoint_Read_16_LE();
    #else
    uint8_t* RequestHeader = (uint8_t*)&USB_ControlRequest;

    for (uint8_t RequestHeaderByte = 0; RequestHeaderByte < sizeof(USB_Request_Header_t); RequestHeaderByte++)
      *(RequestHeader++) = Endpoint_Read_8();
    #endif

    EVENT_USB_Device_ControlRequest();

    if (Endpoint_IsSETUPReceived())
    {
        uint8_t bmRequestType = USB_ControlRequest.bmRequestType;

        switch (USB_ControlRequest.bRequest)
        {
            case REQ_GetStatus:
                if ((bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE)) ||
                    (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_ENDPOINT)))
                {
                    USB_Device_GetStatus();
                }

                break;
            case REQ_ClearFeature:
            case REQ_SetFeature:
                if ((bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE)) ||
                    (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_ENDPOINT)))
                {
                    USB_Device_ClearSetFeature();
                }

                break;
            case REQ_SetAddress:
                if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE))
                  USB_Device_SetAddress();

                break;
            case REQ_GetDescriptor:
                if ((bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE)) ||
                    (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_INTERFACE)))
                {
                    USB_Device_GetDescriptor();
                }

                break;
            case REQ_GetConfiguration:
                if (bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE))
                  USB_Device_GetConfiguration();

                break;
            case REQ_SetConfiguration:
                if (bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE))
                  USB_Device_SetConfiguration();

                break;
        }
    }

    if (Endpoint_IsSETUPReceived())
    {
        Endpoint_StallTransaction();
        Endpoint_ClearSETUP();
    }
}

static void USB_Device_SetAddress(void)
{
    uint8_t DeviceAddress    = (USB_ControlRequest.wValue & 0x7F);
    uint8_t CurrentGlobalInt = SREG;
    cli();

    Endpoint_ClearSETUP();

    Endpoint_ClearStatusStage();

    while (!(Endpoint_IsINReady()));

    USB_Device_SetDeviceAddress(DeviceAddress);
    USB_DeviceState = (DeviceAddress) ? DEVICE_STATE_Addressed : DEVICE_STATE_Default;

    SREG = CurrentGlobalInt;
}

static void USB_Device_SetConfiguration(void)
{
     // was controlled by FIXED_NUM_CONFIGURATIONS
    if ((uint8_t)USB_ControlRequest.wValue > 1) return;

    Endpoint_ClearSETUP();

    USB_Device_ConfigurationNumber = (uint8_t)USB_ControlRequest.wValue;

    Endpoint_ClearStatusStage();

    if (USB_Device_ConfigurationNumber) {
        USB_DeviceState = DEVICE_STATE_Configured;
    } else {
        USB_DeviceState = (USB_Device_IsAddressSet()) ? DEVICE_STATE_Configured : DEVICE_STATE_Powered;
    }

    EVENT_USB_Device_ConfigurationChanged();
}

static void USB_Device_GetConfiguration(void)
{
    Endpoint_ClearSETUP();

    Endpoint_Write_8(USB_Device_ConfigurationNumber);
    Endpoint_ClearIN();

    Endpoint_ClearStatusStage();
}

static void USB_Device_GetDescriptor(void)
{
    const void* DescriptorPointer;
    uint16_t    DescriptorSize;

    DescriptorSize = CALLBACK_USB_GetDescriptor(USB_ControlRequest.wValue, USB_ControlRequest.wIndex, &DescriptorPointer);
    if (DescriptorSize == NO_DESCRIPTOR) return;

    Endpoint_ClearSETUP();

    Endpoint_Write_Control_Stream_LE(DescriptorPointer, DescriptorSize);

    Endpoint_ClearOUT();
}

static void USB_Device_GetStatus(void)
{
    uint8_t CurrentStatus = 0;

    switch (USB_ControlRequest.bmRequestType)
    {
        case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_ENDPOINT):
            Endpoint_SelectEndpoint((uint8_t)USB_ControlRequest.wIndex & ENDPOINT_EPNUM_MASK);

            CurrentStatus = Endpoint_IsStalled();

            Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

            break;
        default:
            return;
    }

    Endpoint_ClearSETUP();

    Endpoint_Write_16_LE(CurrentStatus);
    Endpoint_ClearIN();

    Endpoint_ClearStatusStage();
}

static void USB_Device_ClearSetFeature(void)
{
    switch (USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT)
    {
        case REQREC_ENDPOINT:
            if ((uint8_t)USB_ControlRequest.wValue == FEATURE_SEL_EndpointHalt)
            {
                uint8_t EndpointIndex = ((uint8_t)USB_ControlRequest.wIndex & ENDPOINT_EPNUM_MASK);

                if (EndpointIndex == ENDPOINT_CONTROLEP)
                  return;

                Endpoint_SelectEndpoint(EndpointIndex);

                if (Endpoint_IsEnabled())
                {
                    if (USB_ControlRequest.bRequest == REQ_SetFeature)
                    {
                        Endpoint_StallTransaction();
                    }
                    else
                    {
                        Endpoint_ClearStall();
                        Endpoint_ResetEndpoint(EndpointIndex);
                        Endpoint_ResetDataToggle();
                    }
                }
            }

            break;
        default:
            return;
    }

    Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

    Endpoint_ClearSETUP();

    Endpoint_ClearStatusStage();
}


