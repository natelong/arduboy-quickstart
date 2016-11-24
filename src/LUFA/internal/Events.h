// nate
/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/**
 * USB Event management definitions.
 */

#pragma once

#include "Common.h"

#if !defined(__INCLUDE_FROM_EVENTS_C)
    /** Event for USB device connection. This event fires when the microcontroller is in USB Device mode
     *  and the device is connected to a USB host, beginning the enumeration process measured by a rising
     *  level on the microcontroller's VBUS sense pin.
     *
     *  This event is time-critical; exceeding OS-specific delays within this event handler (typically of around
     *  two seconds) will prevent the device from enumerating correctly.
     *
     *  \note For the microcontrollers with limited USB controller functionality, VBUS sensing is not available.
     *        this means that the current connection state is derived from the bus suspension and wake up events by default,
     *        which is not always accurate (host may suspend the bus while still connected). If the actual connection state
     *        needs to be determined, VBUS should be routed to an external pin, and the auto-detect behaviour turned off by
     *        passing the \c NO_LIMITED_CONTROLLER_CONNECT token to the compiler via the -D switch at compile time. The connection
     *        and disconnection events may be manually fired, and the \ref USB_DeviceState global changed manually.
     *        \n\n
     *
     *  \note This event may fire multiple times during device enumeration on the microcontrollers with limited USB controllers
     *        if \c NO_LIMITED_CONTROLLER_CONNECT is not defined.
     *
     *  \see \ref Group_USBManagement for more information on the USB management task and reducing CPU usage.
     */
    void EVENT_USB_Device_Connect(void);

    /** Event for USB device disconnection. This event fires when the microcontroller is in USB Device mode and the device is
     *  disconnected from a host, measured by a falling level on the microcontroller's VBUS sense pin.
     *
     *  \note For the microcontrollers with limited USB controllers, VBUS sense is not available to the USB controller.
     *        this means that the current connection state is derived from the bus suspension and wake up events by default,
     *        which is not always accurate (host may suspend the bus while still connected). If the actual connection state
     *        needs to be determined, VBUS should be routed to an external pin, and the auto-detect behaviour turned off by
     *        passing the \c NO_LIMITED_CONTROLLER_CONNECT token to the compiler via the -D switch at compile time. The connection
     *        and disconnection events may be manually fired, and the \ref USB_DeviceState global changed manually.
     *        \n\n
     *
     *  \note This event may fire multiple times during device enumeration on the microcontrollers with limited USB controllers
     *        if \c NO_LIMITED_CONTROLLER_CONNECT is not defined.
     *
     *  \see \ref Group_USBManagement for more information on the USB management task and reducing CPU usage.
     */
    void EVENT_USB_Device_Disconnect(void);

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

    /** Event for USB suspend. This event fires when a the USB host suspends the device by halting its
     *  transmission of Start Of Frame pulses to the device. This is generally hooked in order to move
     *  the device over to a low power state until the host wakes up the device. If the USB interface is
     *  enumerated with the \ref USB_OPT_AUTO_PLL option set, the library will automatically suspend the
     *  USB PLL before the event is fired to save power.
     *
     *  \note This event does not exist if the \c USB_HOST_ONLY token is supplied to the compiler (see
     *        \ref Group_USBManagement documentation).
     *        \n\n
     *
     *  \note This event does not exist on the microcontrollers with limited USB VBUS sensing abilities
     *        when the \c NO_LIMITED_CONTROLLER_CONNECT compile time token is not set - see
     *        \ref EVENT_USB_Device_Disconnect.
     *
     *  \see \ref EVENT_USB_Device_WakeUp() event for accompanying Wake Up event.
     */
    void EVENT_USB_Device_Suspend(void);

    /** Event for USB wake up. This event fires when a the USB interface is suspended while in device
     *  mode, and the host wakes up the device by supplying Start Of Frame pulses. This is generally
     *  hooked to pull the user application out of a low power state and back into normal operating
     *  mode. If the USB interface is enumerated with the \ref USB_OPT_AUTO_PLL option set, the library
     *  will automatically restart the USB PLL before the event is fired.
     *
     *  \note This event does not exist if the \c USB_HOST_ONLY token is supplied to the compiler (see
     *        \ref Group_USBManagement documentation).
     *        \n\n
     *
     *  \note This event does not exist on the microcontrollers with limited USB VBUS sensing abilities
     *        when the \c NO_LIMITED_CONTROLLER_CONNECT compile time token is not set - see
     *        \ref EVENT_USB_Device_Disconnect.
     *
     *  \see \ref EVENT_USB_Device_Suspend() event for accompanying Suspend event.
     */
    void EVENT_USB_Device_WakeUp(void);

    /** Event for USB interface reset. This event fires when the USB interface is in device mode, and
     *  a the USB host requests that the device reset its interface. This event fires after the control
     *  endpoint has been automatically configured by the library.
     *
     *  This event is time-critical; exceeding OS-specific delays within this event handler (typically of around
     *  two seconds) will prevent the device from enumerating correctly.
     *
     *  \note This event does not exist if the \c USB_HOST_ONLY token is supplied to the compiler (see
     *        \ref Group_USBManagement documentation).
     */
    void EVENT_USB_Device_Reset(void);

    /** Event for USB Start Of Frame detection, when enabled. This event fires at the start of each USB
     *  frame, once per millisecond, and is synchronized to the USB bus. This can be used as an accurate
     *  millisecond timer source when the USB bus is enumerated in device mode to a USB host.
     *
     *  This event is time-critical; it is run once per millisecond and thus long handlers will significantly
     *  degrade device performance. This event should only be enabled when needed to reduce device wake-ups.
     *
     *  \pre This event is not normally active - it must be manually enabled and disabled via the
     *       \ref USB_Device_EnableSOFEvents() and \ref USB_Device_DisableSOFEvents() commands after enumeration.
     *       \n\n
     *
     *  \note This event does not exist if the \c USB_HOST_ONLY token is supplied to the compiler (see
     *        \ref Group_USBManagement documentation).
     */
    void EVENT_USB_Device_StartOfFrame(void);
#else
    void USB_Event_Stub(void);

    void EVENT_USB_Device_Connect(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_Disconnect(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_ControlRequest(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_ConfigurationChanged(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_Suspend(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_WakeUp(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_Reset(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
    void EVENT_USB_Device_StartOfFrame(void) ATTR_WEAK ATTR_ALIAS(USB_Event_Stub);
#endif
