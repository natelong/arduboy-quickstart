/**
 * Common USB Controller definitions for all architectures.
 */

#pragma once

#include "../../ab_common.h"

/** Indicates the currently set configuration number of the device. USB devices may have several
 *  different configurations which the host can select between; this indicates the currently selected
 *  value, or 0 if no configuration has been selected.
 */
extern uint8_t USB_Device_ConfigurationNumber;

// Standard USB Control Request
typedef struct {
    uint8_t  type;    // Type of the request.
    uint8_t  request; // Request command code.
    uint16_t value;   // value parameter of the request.
    uint16_t index;   // index parameter of the request.
    uint16_t length;  // Length of the data to transfer in bytes.
} PACKED USB_Request_Header_t;

extern USB_Request_Header_t USB_ControlRequest;

#define USB_DeviceState GPIOR0
