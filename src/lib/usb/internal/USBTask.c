#include "USBTask.h"

USB_Request_Header_t USB_ControlRequest;

void USB_USBTask(void) {
    if (USB_DeviceState != DEVICE_STATE_Unattached) {
        uint8_t PrevEndpoint = Endpoint_GetCurrentEndpoint();
        Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
        if (Endpoint_IsSETUPReceived()) USB_Device_ProcessControlRequest();
        Endpoint_SelectEndpoint(PrevEndpoint);
    }
}
