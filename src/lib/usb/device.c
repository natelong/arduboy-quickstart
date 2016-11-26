#include "device.h"

#define DTH_STD_DEV (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE)
#define DTH_STD_EP  (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_ENDPOINT)
#define DTH_STD_IF  (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_INTERFACE)
#define HTD_STD_DEV (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_DEVICE)
#define HTD_STD_EP  (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_ENDPOINT)

static void USB_Device_SetAddress(void);
static void USB_Device_SetConfiguration(void);
static void USB_Device_GetConfiguration(void);
static void USB_Device_GetDescriptor(void);
static void USB_Device_GetStatus(void);
static void USB_Device_ClearSetFeature(void);

void USB_Device_ProcessControlRequest(void) {
    uint8_t* RequestHeader = (uint8_t*)&USB_ControlRequest;

    for (uint8_t RequestHeaderByte = 0; RequestHeaderByte < sizeof(USB_Request_Header_t); RequestHeaderByte++) {
        *(RequestHeader++) = Endpoint_Read_8();
    }

    EVENT_USB_Device_ControlRequest();

    if (Endpoint_IsSETUPReceived()) {
        uint8_t type = USB_ControlRequest.type;

        switch (USB_ControlRequest.request) {
            case REQ_GetStatus:
                if ((type == DTH_STD_DEV) || (type == DTH_STD_EP))  USB_Device_GetStatus();
                break;
            case REQ_ClearFeature:
            case REQ_SetFeature:
                if ((type == HTD_STD_DEV) || (type == HTD_STD_EP)) USB_Device_ClearSetFeature();
                break;
            case REQ_SetAddress:
                if (type == HTD_STD_DEV) USB_Device_SetAddress();
                break;
            case REQ_GetDescriptor:
                if ((type == DTH_STD_DEV) || (type == DTH_STD_IF)) USB_Device_GetDescriptor();
                break;
            case REQ_GetConfiguration:
                if (type == DTH_STD_DEV) USB_Device_GetConfiguration();
                break;
            case REQ_SetConfiguration:
                if (type == HTD_STD_DEV) USB_Device_SetConfiguration();
                break;
        }
    }

    if (Endpoint_IsSETUPReceived()) {
        Endpoint_StallTransaction();
        Endpoint_ClearSETUP();
    }
}

static void USB_Device_SetAddress(void) {
    uint8_t DeviceAddress    = (USB_ControlRequest.value & 0x7F);
    uint8_t CurrentGlobalInt = SREG;
    cli();
    Endpoint_ClearSETUP();
    Endpoint_ClearStatusStage();
    while (!(Endpoint_IsINReady()));
    { // USB_Device_SetDeviceAddress(DeviceAddress);
        uint8_t Temp = (UDADDR & (1 << ADDEN)) | (DeviceAddress & 0x7F);
        UDADDR = Temp;
        UDADDR = Temp | (1 << ADDEN);
    }
    USB_DeviceState = (DeviceAddress) ? DEVICE_STATE_Addressed : DEVICE_STATE_Default;
    SREG = CurrentGlobalInt;
}

static void USB_Device_SetConfiguration(void) {
    // was controlled by FIXED_NUM_CONFIGURATIONS
    if ((uint8_t)USB_ControlRequest.value > 1) return;
    Endpoint_ClearSETUP();
    USB_Device_ConfigurationNumber = (uint8_t)USB_ControlRequest.value;
    Endpoint_ClearStatusStage();
    if (USB_Device_ConfigurationNumber) {
        USB_DeviceState = DEVICE_STATE_Configured;
    } else {
        USB_DeviceState = (USB_Device_IsAddressSet()) ? DEVICE_STATE_Configured : DEVICE_STATE_Powered;
    }
    EVENT_USB_Device_ConfigurationChanged();
}

static void USB_Device_GetConfiguration(void) {
    Endpoint_ClearSETUP();
    Endpoint_Write_8(USB_Device_ConfigurationNumber);
    Endpoint_ClearIN();
    Endpoint_ClearStatusStage();
}

static void USB_Device_GetDescriptor(void) {
    const void* DescriptorPointer;
    uint16_t    DescriptorSize;
    DescriptorSize = CALLBACK_USB_GetDescriptor(USB_ControlRequest.value, &DescriptorPointer);
    if (DescriptorSize == NO_DESCRIPTOR) return;

    Endpoint_ClearSETUP();
    Endpoint_Write_Control_Stream(DescriptorPointer, DescriptorSize);
    Endpoint_ClearOUT();
}

static void USB_Device_GetStatus(void) {
    uint8_t CurrentStatus = 0;
    switch (USB_ControlRequest.type) {
        case DTH_STD_EP:
            Endpoint_SelectEndpoint((uint8_t)USB_ControlRequest.index & ENDPOINT_EPNUM_MASK);
            CurrentStatus = Endpoint_IsStalled();
            Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
            break;
        default:
            return;
    }

    Endpoint_ClearSETUP();
    Endpoint_Write_16(CurrentStatus);
    Endpoint_ClearIN();
    Endpoint_ClearStatusStage();
}

static void USB_Device_ClearSetFeature(void) {
    switch (USB_ControlRequest.type & CONTROL_REQTYPE_RECIPIENT) {
        case REQREC_ENDPOINT:
            if ((uint8_t)USB_ControlRequest.value == FEATURE_SEL_EndpointHalt) {
                uint8_t EndpointIndex = ((uint8_t)USB_ControlRequest.index & ENDPOINT_EPNUM_MASK);

                if (EndpointIndex == ENDPOINT_CONTROLEP) return;

                Endpoint_SelectEndpoint(EndpointIndex);

                if (Endpoint_IsEnabled()) {
                    if (USB_ControlRequest.request == REQ_SetFeature) {
                        Endpoint_StallTransaction();
                    } else {
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