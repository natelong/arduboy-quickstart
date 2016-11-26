/**
 * USB Event management definitions.
 */

#pragma once

#include "../../ab_common.h"

#if !defined(__INCLUDE_FROM_EVENTS_C)
    /** Event for control requests. This event fires when a the USB host issues a control request
     *  to the mandatory device control endpoint (of address 0). This may either be a standard
     *  request that the library may have a handler code for internally, or a class specific request
     *  issued to the device which must be handled appropriately. If a request is not processed in the
     *  user application via this event, it will be passed to the library for processing internally
     *  if a suitable handler exists.
     *
     *  This event is time-critical; each packet within the request transaction must be acknowledged or
     *  sent within 50ms or the host will abort the transfer.
     *
     *  The library internally handles all standard control requests with the exceptions of SYNC FRAME,
     *  SET DESCRIPTOR and SET INTERFACE. These and all other non-standard control requests will be left
     *  for the user to process via this event if desired. If not handled in the user application or by
     *  the library internally, unknown requests are automatically STALLed.
     *
     *  \note This event does not exist if the \c USB_HOST_ONLY token is supplied to the compiler (see
     *        \ref Group_USBManagement documentation).
     *        \n\n
     *
     *  \note Requests should be handled in the same manner as described in the USB 2.0 Specification,
     *        or appropriate class specification. In all instances, the library has already read the
     *        request SETUP parameters into the \ref USB_ControlRequest structure which should then be used
     *        by the application to determine how to handle the issued request.
     */
    void EVENT_USB_Device_ControlRequest(void);

    /** Event for USB configuration number changed. This event fires when a the USB host changes the
     *  selected configuration number while in device mode. This event should be hooked in device
     *  applications to create the endpoints and configure the device for the selected configuration.
     *
     *  This event is time-critical; exceeding OS-specific delays within this event handler (typically of around
     *  one second) will prevent the device from enumerating correctly.
     *
     *  This event fires after the value of \ref USB_Device_ConfigurationNumber has been changed.
     *
     *  \note This event does not exist if the \c USB_HOST_ONLY token is supplied to the compiler (see
     *        \ref Group_USBManagement documentation).
     */
    void EVENT_USB_Device_ConfigurationChanged(void);
#else
    void USB_Event_Stub(void);

    void EVENT_USB_Device_ControlRequest(void)       __attribute__ ((weak)) __attribute__ ((alias( "USB_Event_Stub" )));
    void EVENT_USB_Device_ConfigurationChanged(void) __attribute__ ((weak)) __attribute__ ((alias( "USB_Event_Stub" )));
#endif
