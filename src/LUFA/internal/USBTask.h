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

/** \file
 *  \brief Main USB service task management.
 *
 *  This file contains the function definitions required for the main USB service task, which must be called
 *  from the user application to ensure that the USB connection to or from a connected USB device is maintained.
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in LUFA/Drivers/USB/USB.h.
 */

#pragma once

#include "Common.h"
#include "USBController.h"
#include "Events.h"
#include "StdRequestType.h"
#include "StdDescriptors.h"
#include "DeviceStandardReq.h"

/** Indicates if the USB interface is currently initialized but not necessarily connected to a host
 *  or device (i.e. if USB_Init() has been run). If this is false, all other library globals related
 *  to the USB driver are invalid.
 *
 *  Note: This variable should be treated as read-only in the user application, and never manually
 *        changed in value.
 */
extern volatile bool USB_IsInitialized;

/** Structure containing the last received Control request when in Device mode (for use in user-applications
 *  inside of the EVENT_USB_Device_ControlRequest() event.
 *
 *  Note: The contents of this structure is automatically endian-corrected for the current CPU architecture.
 */
extern USB_Request_Header_t USB_ControlRequest;

#define _GET_DEVICE_GPIOR_NAME2(y) GPIOR ## y
#define _GET_DEVICE_GPIOR_NAME(x)  _GET_DEVICE_GPIOR_NAME2(x)
#define USB_DeviceState            _GET_DEVICE_GPIOR_NAME(0) // TODO: figure out what this does

/** This is the main USB management task. The USB driver requires this task to be executed
 *  continuously when the USB system is active (device attached in host mode, or attached to a host
 *  in device mode) in order to manage USB communications. This task may be executed inside an RTOS,
 *  fast timer ISR or the main user application loop.
 *
 *  The USB task must be serviced within 30ms while in device mode. The task may be serviced at all times, or
 *  (for minimum CPU consumption) it may be disabled at start-up, enabled on the firing of the EVENT_USB_Device_Connect()
 *  event and disabled again on the firing of the EVENT_USB_Device_Disconnect() event.
 *
 *  If in device mode (only), the control endpoint can instead be managed via interrupts entirely by the library
 *  by defining the INTERRUPT_CONTROL_ENDPOINT token and passing it to the compiler via the -D switch.
 */
void USB_USBTask(void);

#define HOST_TASK_NONBLOCK_WAIT(Duration, NextState) MACROS{ USB_HostState   = HOST_STATE_WaitForDevice; \
                                                             WaitMSRemaining = (Duration);               \
                                                             PostWaitState   = (NextState);              }MACROE
