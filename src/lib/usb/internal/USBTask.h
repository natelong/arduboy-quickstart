/**
 *  Main USB service task management.
 */

#pragma once

#include "../../ab_common.h"
#include "usb.h"
#include "Events.h"
#include "StdRequestType.h"
#include "StdDescriptors.h"
#include "DeviceStandardReq.h"

/**
 * Structure containing the last received Control request when in Device mode (for use in user-applications
 * inside of the EVENT_USB_Device_ControlRequest() event.
 */
extern USB_Request_Header_t USB_ControlRequest;

#define USB_DeviceState GPIOR0

/**
 * This is the main USB management task. The USB driver requires this task to be executed continuously when the USB
 * system is active in order to manage USB communications. This task may be executed inside an RTOS,
 * fast timer ISR or the main user application loop.
 *
 * The USB task must be serviced within 30ms while in device mode. The task may be serviced at all times, or
 * (for minimum CPU consumption) it may be disabled at start-up, enabled on the firing of the EVENT_USB_Device_Connect()
 * event and disabled again on the firing of the EVENT_USB_Device_Disconnect() event.
 */
void USB_USBTask(void);
