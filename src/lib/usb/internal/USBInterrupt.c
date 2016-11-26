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
    if (USB_INT_HasVBUSOccurred()) {
        USB_INT_ClearVBUS();

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

    if (USB_INT_HasSuspendOccurred()) {
        USB_INT_DisableSuspend();
        USB_INT_EnableWakeup();
        USB_CLK_Freeze();
        USB_PLL_Off();
        USB_DeviceState = DEVICE_STATE_Suspended;
        EVENT_USB_Device_Suspend();
    }

    if (USB_INT_HasWakeupOccurred()) {
        USB_PLL_On();
        while (!(USB_PLL_IsReady()));
        USB_CLK_Unfreeze();
        USB_INT_ClearWakeup();
        USB_INT_DisableWakeup();
        USB_INT_EnableSuspend();
        if (USB_Device_ConfigurationNumber) {
            USB_DeviceState = DEVICE_STATE_Configured;
        } else {
            USB_DeviceState = (USB_Device_IsAddressSet()) ? DEVICE_STATE_Configured : DEVICE_STATE_Powered;
        }
        EVENT_USB_Device_WakeUp();
    }

    if (USB_INT_HasResetOccurred()) {
        USB_INT_ClearReset();

        USB_DeviceState                = DEVICE_STATE_Default;
        USB_Device_ConfigurationNumber = 0;

        USB_INT_ClearSuspend();
        USB_INT_DisableSuspend();
        USB_INT_EnableWakeup();

        Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL,
                                   ENDPOINT_DIR_OUT, USB_Device_ControlEndpointSize,
                                   ENDPOINT_BANK_SINGLE);

        EVENT_USB_Device_Reset();
    }
}
