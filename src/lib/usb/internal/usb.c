#include "usb.h"

void USB_Init() {
    UHWCON  |=  (1 << UVREGE); // USB_REG_On();
    PLLFRQ = ((1 << PLLUSB) | (1 << PDIV3) | (1 << PDIV1));
    USB_INT_DisableAllInterrupts();
    USB_INT_ClearAllInterrupts();
    USBCON &= ~(1 << USBE); // USB_Controller_Reset();
    USBCON |=  (1 << USBE); // USB_Controller_Reset();
    USB_CLK_Unfreeze();
    USB_PLL_Off();
    USB_DeviceState = DEVICE_STATE_Unattached;
    USB_Device_ConfigurationNumber = 0;
    UDCON &= ~(1 << LSM); // USB_Device_SetFullSpeed();
    USB_INT_EnableVBUS();
    Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL, ENDPOINT_DIR_OUT, USB_Device_ControlEndpointSize, ENDPOINT_BANK_SINGLE);
    USB_INT_ClearSuspend();
    USB_INT_EnableSuspend();
    USB_INT_EnableReset();
    UDCON  &= ~(1 << DETACH); // USB_Attach();
    USBCON  |=  (1 << OTGPADE); // USB_OTGPAD_On();
}

void USB_Disable(void) {
    USB_INT_DisableAllInterrupts();
    USB_INT_ClearAllInterrupts();
    UDCON  |=  (1 << DETACH);   // USB_Detach();
    USBCON  &= ~(1 << USBE);    // USB_Controller_Disable();
    USB_PLL_Off();
    UHWCON  &= ~(1 << UVREGE);  // USB_REG_Off();
    USBCON  &= ~(1 << OTGPADE); // USB_OTGPAD_Off();
}
