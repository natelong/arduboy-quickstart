#pragma once

#include "CDC.h"

#define AVR_SIGNATURE_1 0x1E
#define AVR_SIGNATURE_2 0x95
#define AVR_SIGNATURE_3 0x87

#define CDC_NOTIFICATION_EPNUM   2 // Endpoint number for the CDC control interface event notification endpoint.
#define CDC_TX_EPNUM             3 // Endpoint number for the CDC data interface TX (data IN) endpoint.
#define CDC_RX_EPNUM             4 // Endpoint number for the CDC data interface RX (data OUT) endpoint.
#define CDC_TXRX_EPSIZE         16 // Size of the CDC data interface TX and RX data endpoint banks, in bytes.
#define CDC_NOTIFICATION_EPSIZE  8 // Size of the CDC control interface notification endpoint bank, in bytes.

/**
 * Type define for the device configuration descriptor structure. This must be defined in the
 * application code, as the configuration descriptor contains several sub-descriptors which
 * vary between devices, and which describe the device's usage to the host.
 */
typedef struct {
        USB_Descriptor_Configuration_Header_t Config;
        // CDC Control Interface
        USB_Descriptor_Interface_t            CDC_CCI_Interface;
        USB_CDC_Descriptor_FunctionalHeader_t CDC_Functional_Header;
        USB_CDC_Descriptor_FunctionalACM_t    CDC_Functional_ACM;
        USB_CDC_Descriptor_FunctionalUnion_t  CDC_Functional_Union;
        USB_Descriptor_Endpoint_t             CDC_NotificationEndpoint;
        // CDC Data Interface
        USB_Descriptor_Interface_t            CDC_DCI_Interface;
        USB_Descriptor_Endpoint_t             CDC_DataOutEndpoint;
        USB_Descriptor_Endpoint_t             CDC_DataInEndpoint;
} USB_Descriptor_Configuration_t;
