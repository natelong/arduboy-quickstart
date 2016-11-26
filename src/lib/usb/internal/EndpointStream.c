#include "EndpointStream.h"

uint8_t Endpoint_Write_Control_Stream(const void* const Buffer, uint16_t Length) {
    uint8_t* DataStream     = ((uint8_t*)Buffer);
    bool     LastPacketFull = false;

    if (Length > USB_ControlRequest.length) {
        Length = USB_ControlRequest.length;
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

uint8_t Endpoint_Read_Control_Stream(void* const Buffer, uint16_t Length) {
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