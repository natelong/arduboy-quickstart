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
 * USB control endpoint request definitions.
 */

#pragma once

#include "../../ab_common.h"

/** Mask for the request type parameter, to indicate the direction of the request data (Host to Device
 *  or Device to Host). The result of this mask should then be compared to the request direction masks.
 */
#define CONTROL_REQTYPE_DIRECTION  0x80

/** Mask for the request type parameter, to indicate the type of request (Device, Class or Vendor
 *  Specific). The result of this mask should then be compared to the request type masks.
 */
#define CONTROL_REQTYPE_TYPE       0x60

/** Mask for the request type parameter, to indicate the recipient of the request (Device, Interface
 *  Endpoint or Other). The result of this mask should then be compared to the request recipient
 *  masks.
 */
#define CONTROL_REQTYPE_RECIPIENT  0x1F

// Request data direction mask, indicating that the request data will flow from host to device.
#define REQDIR_HOSTTODEVICE        (0 << 7)

// Request data direction mask, indicating that the request data will flow from device to host.
#define REQDIR_DEVICETOHOST        (1 << 7)

// Request type mask, indicating that the request is a standard request.
#define REQTYPE_STANDARD           (0 << 5)

// Request type mask, indicating that the request is a class-specific request.
#define REQTYPE_CLASS              (1 << 5)

// Request type mask, indicating that the request is a vendor specific request.
#define REQTYPE_VENDOR             (2 << 5)

// Request recipient mask, indicating that the request is to be issued to the device as a whole.
#define REQREC_DEVICE              (0 << 0)

/** Request recipient mask, indicating that the request is to be issued to an interface in the
 *  currently selected configuration.
 */
#define REQREC_INTERFACE           (1 << 0)

/** Request recipient mask, indicating that the request is to be issued to an endpoint in the
 *  currently selected configuration.
 */
#define REQREC_ENDPOINT            (2 << 0)

/** Request recipient mask, indicating that the request is to be issued to an unspecified element
 *  in the currently selected configuration.
 */
#define REQREC_OTHER               (3 << 0)

/**
 * Standard USB Control Request
 */
typedef struct {
    uint8_t  bmRequestType; // Type of the request.
    uint8_t  bRequest;      // Request command code.
    uint16_t wValue;        // wValue parameter of the request.
    uint16_t wIndex;        // wIndex parameter of the request.
    uint16_t wLength;       // Length of the data to transfer in bytes.
} PACKED USB_Request_Header_t;

/** Enumeration for the various standard request commands. These commands are applicable when the
 *  request type is \ref REQTYPE_STANDARD (with the exception of \ref REQ_GetDescriptor, which is always
 *  handled regardless of the request type value).
 */
enum USB_Control_Request_t {
    REQ_GetStatus        = 0, /**  Implemented in the library for device and endpoint recipients. Passed
                               *   to the user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_ClearFeature     = 1, /**  Implemented in the library for device and endpoint recipients. Passed
                               *   to the user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_SetFeature       = 3, /**  Implemented in the library for device and endpoint recipients. Passed
                               *   to the user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_SetAddress       = 5, /**  Implemented in the library for the device recipient. Passed
                               *   to the user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_GetDescriptor    = 6, /**  Implemented in the library for device and interface recipients. Passed to the
                               *   user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_SetDescriptor    = 7, /**  Not implemented in the library, passed to the user application
                               *   via the EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_GetConfiguration = 8, /**  Implemented in the library for the device recipient. Passed
                               *   to the user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_SetConfiguration = 9, /**  Implemented in the library for the device recipient. Passed
                               *   to the user application for other recipients via the
                               *   EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_GetInterface     = 10, /** Not implemented in the library, passed to the user application
                               *   via the EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_SetInterface     = 11, /** Not implemented in the library, passed to the user application
                               *   via the EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
    REQ_SynchFrame       = 12, /** Not implemented in the library, passed to the user application
                               *   via the EVENT_USB_Device_ControlRequest() event when received in
                               *   device mode. */
};

/** Feature Selector values for Set Feature and Clear Feature standard control requests directed to the device, interface
 *  and endpoint recipients.
 */
enum USB_Feature_Selectors_t {
    FEATURE_SEL_EndpointHalt       = 0x00, /**< Feature selector for Clear Feature or Set Feature commands. When
                                            *   used in a Set Feature or Clear Feature request this indicates that an
                                            *   endpoint (whose address is given elsewhere in the request) should have
                                            *   its stall condition changed. */
    FEATURE_SEL_DeviceRemoteWakeup = 0x01, /**< Feature selector for Device level Remote Wakeup enable set or clear.
                                            *   This feature can be controlled by the host on devices which indicate
                                            *   remote wakeup support in their descriptors to selectively disable or
                                            *   enable remote wakeup. */
    FEATURE_SEL_TestMode           = 0x02, /**< Feature selector for Test Mode features, used to test the USB controller
                                            *   to check for incorrect operation. */
};

#define FEATURE_SELFPOWERED_ENABLED     (1 << 0)
#define FEATURE_REMOTE_WAKEUP_ENABLED   (1 << 1)
