#include "Endpoint.h"

#define USB_STREAM_TIMEOUT_MS 100

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
    if (USB_ControlRequest.type & REQDIR_DEVICETOHOST) {
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
        if (Endpoint_GetEndpointDirection() == EP_DIR_IN) {
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
