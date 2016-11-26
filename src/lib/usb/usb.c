#include "usb.h"

USB_Request_Header_t USB_ControlRequest;
uint8_t              USB_Device_ConfigurationNumber;

void EVENT_USB_Device_ControlRequest(void) {}
void EVENT_USB_Device_ConfigurationChanged(void) {}
