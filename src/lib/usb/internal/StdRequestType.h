/**
 * USB control endpoint request definitions.
 */

#pragma once

#include "../../ab_common.h"

#define CONTROL_REQTYPE_DIRECTION 0x80
#define CONTROL_REQTYPE_TYPE      0x60
#define CONTROL_REQTYPE_RECIPIENT 0x1F

#define REQDIR_HOSTTODEVICE (0 << 7)
#define REQDIR_DEVICETOHOST (1 << 7)

#define REQTYPE_STANDARD    (0 << 5)
#define REQTYPE_CLASS       (1 << 5)
#define REQTYPE_VENDOR      (2 << 5)

#define REQREC_DEVICE    (0 << 0)
#define REQREC_INTERFACE (1 << 0)
#define REQREC_ENDPOINT  (2 << 0)
#define REQREC_OTHER     (3 << 0)

#define REQ_GetStatus            0
#define REQ_ClearFeature         1
#define REQ_SetFeature           3
#define REQ_SetAddress           5
#define REQ_GetDescriptor        6
#define REQ_GetConfiguration     8
#define REQ_SetConfiguration     9

#define FEATURE_SEL_EndpointHalt 0x00

// Standard USB Control Request
typedef struct {
    uint8_t  bmRequestType; // Type of the request.
    uint8_t  bRequest;      // Request command code.
    uint16_t wValue;        // wValue parameter of the request.
    uint16_t wIndex;        // wIndex parameter of the request.
    uint16_t wLength;       // Length of the data to transfer in bytes.
} PACKED USB_Request_Header_t;
