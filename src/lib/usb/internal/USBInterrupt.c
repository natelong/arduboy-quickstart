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
            USB_PLL_On();
            while (!(USB_PLL_IsReady()));
            USB_DeviceState = DEVICE_STATE_Powered;
            EVENT_USB_Device_Connect();
        } else {
            USB_PLL_Off();
            USB_DeviceState = DEVICE_STATE_Unattached;
            EVENT_USB_Device_Disconnect();
        }
    }

    if (USB_INT_HasOccurred(USB_INT_SUSPI) && USB_INT_IsEnabled(USB_INT_SUSPI)) {
        USB_INT_Disable(USB_INT_SUSPI);
        USB_INT_Enable(USB_INT_WAKEUPI);
        USB_CLK_Freeze();
        USB_PLL_Off();
        USB_DeviceState = DEVICE_STATE_Suspended;
        EVENT_USB_Device_Suspend();
    }

    if (USB_INT_HasOccurred(USB_INT_WAKEUPI) && USB_INT_IsEnabled(USB_INT_WAKEUPI)) {
        USB_PLL_On();
        while (!(USB_PLL_IsReady()));
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
