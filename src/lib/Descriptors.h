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

#pragma once

#include <LUFA/USB.h>

#define AVR_SIGNATURE_1 0x1E
#define AVR_SIGNATURE_2 0x95
#define AVR_SIGNATURE_3 0x87

#define CDC_NOTIFICATION_EPNUM   2 // Endpoint number for the CDC control interface event notification endpoint.
#define CDC_TX_EPNUM             3 // Endpoint number for the CDC data interface TX (data IN) endpoint.
#define CDC_RX_EPNUM             4 // Endpoint number for the CDC data interface RX (data OUT) endpoint.
#define CDC_TXRX_EPSIZE         16 // Size of the CDC data interface TX and RX data endpoint banks, in bytes.
#define CDC_NOTIFICATION_EPSIZE  8 // Size of the CDC control interface notification endpoint bank, in bytes.

/** Type define for the device configuration descriptor structure. This must be defined in the
 *  application code, as the configuration descriptor contains several sub-descriptors which
 *  vary between devices, and which describe the device's usage to the host.
 */
typedef struct {
        USB_Descriptor_Configuration_Header_t    Config;

        // CDC Control Interface
        USB_Descriptor_Interface_t               CDC_CCI_Interface;
        USB_CDC_Descriptor_FunctionalHeader_t    CDC_Functional_Header;
        USB_CDC_Descriptor_FunctionalACM_t       CDC_Functional_ACM;
        USB_CDC_Descriptor_FunctionalUnion_t     CDC_Functional_Union;
        USB_Descriptor_Endpoint_t                CDC_NotificationEndpoint;

        // CDC Data Interface
        USB_Descriptor_Interface_t               CDC_DCI_Interface;
        USB_Descriptor_Endpoint_t                CDC_DataOutEndpoint;
        USB_Descriptor_Endpoint_t                CDC_DataInEndpoint;
} USB_Descriptor_Configuration_t;

/* Function Prototypes: */
uint16_t CALLBACK_USB_GetDescriptor(
    const uint16_t wValue,
    const uint8_t wIndex,
    const void** const DescriptorAddress
);
