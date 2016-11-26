/**
 *  Master include file for the library USB CDC-ACM Class driver.
 */

#pragma once

#include "StdDescriptors.h"

// Enum for possible Class, Subclass and Protocol values of device and interface descriptors
enum CDC_Descriptor_ClassSubclassProtocol_t {
    CDC_CSCP_CDCClass               = 0x02, // device or interface
    CDC_CSCP_NoSpecificSubclass     = 0x00, // device or interface belongs to no specific subclass of the CDC class.
    CDC_CSCP_ACMSubclass            = 0x02, // device or interface belongs to the Abstract Control Model CDC subclass.
    CDC_CSCP_ATCommandProtocol      = 0x01, // device or interface belongs to the AT Command protocol of the CDC class.
    CDC_CSCP_NoSpecificProtocol     = 0x00, // device or interface belongs to no specific protocol of the CDC class.
    CDC_CSCP_VendorSpecificProtocol = 0xFF, // device or interface belongs to a vendor-specific protocol of the CDC class.
    CDC_CSCP_CDCDataClass           = 0x0A, // device or interface belongs to the CDC Data class.
    CDC_CSCP_NoDataSubclass         = 0x00, // device or interface belongs to no specific subclass of the CDC data class.
    CDC_CSCP_NoDataProtocol         = 0x00, // device or interface belongs to no specific protocol of the CDC data class.
};

// Enum for the CDC class specific control requests that can be issued by the USB bus host.
enum CDC_ClassRequests_t {
    CDC_REQ_SendEncapsulatedCommand = 0x00, // send an encapsulated command to the device.
    CDC_REQ_GetEncapsulatedResponse = 0x01, // retrieve an encapsulated command response from the device.
    CDC_REQ_SetLineEncoding         = 0x20, // set the current virtual serial port configuration settings.
    CDC_REQ_GetLineEncoding         = 0x21, // get the current virtual serial port configuration settings.
    CDC_REQ_SetControlLineState     = 0x22, // set the current virtual serial port handshake line states.
    CDC_REQ_SendBreak               = 0x23, // send a break to the receiver via the carrier channel.
};

// Enum for the possible line encoding formats of a virtual serial port.
enum CDC_LineEncodingFormats_t {
    CDC_LINEENCODING_OneStopBit          = 0, // Each frame contains one stop bit.
    CDC_LINEENCODING_OneAndAHalfStopBits = 1, // Each frame contains one and a half stop bits.
    CDC_LINEENCODING_TwoStopBits         = 2, // Each frame contains two stop bits.
};

// Enum for the possible line encoding parity settings of a virtual serial port.
enum CDC_LineEncodingParity_t {
    CDC_PARITY_None  = 0, // No parity bit mode on each frame.
    CDC_PARITY_Odd   = 1, // Odd parity bit mode on each frame.
    CDC_PARITY_Even  = 2, // Even parity bit mode on each frame.
    CDC_PARITY_Mark  = 3, // Mark parity bit mode on each frame.
    CDC_PARITY_Space = 4, // Space parity bit mode on each frame.
};

/**
 *  Type define for a CDC class-specific functional header descriptor. This indicates to the host that the device
 *  contains one or more CDC functional data descriptors, which give the CDC interface's capabilities and configuration.
 *  See the CDC class specification for more details.
 */
typedef struct {
    USB_Descriptor_Header_t Header;           // Regular descriptor header containing the descriptor's type and length.
    uint8_t                 Subtype;          // Sub type value used to distinguish between CDC class-specific descriptors
                                              // must be CDC_DSUBTYPE_CSInterface_Header.
    uint16_t                CDCSpecification; // Version number of the CDC specification implemented by the device, encoded
                                              //   in BCD format.
} PACKED USB_CDC_Descriptor_FunctionalHeader_t;


/**
 *  Type define for a CDC class-specific functional ACM descriptor. This indicates to the host that the CDC interface
 *  supports the CDC ACM subclass of the CDC specification. See the CDC class specification for more details.
 */
typedef struct {
    USB_Descriptor_Header_t Header;       // Regular descriptor header containing the descriptor's type and length. */
    uint8_t                 Subtype;      // Sub type value used to distinguish between CDC class-specific descriptors,
                                          //   must be \ref CDC_DSUBTYPE_CSInterface_ACM.
    uint8_t                 Capabilities; // Capabilities of the ACM interface, given as a bit mask. For most devices,
                                          //   this should be set to a fixed value of 0x06 - for other capabilities, refer
                                          //   to the CDC ACM specification.
} PACKED USB_CDC_Descriptor_FunctionalACM_t;

/**
 *  Type define for a CDC class-specific functional Union descriptor. This indicates to the host that specific
 *  CDC control and data interfaces are related. See the CDC class specification for more details.
 */
typedef struct {
    USB_Descriptor_Header_t Header;                // Regular descriptor header containing the descriptor's type and length.
    uint8_t                 Subtype;               // Sub type value used to distinguish between CDC class-specific descriptors,
                                                   //   must be \ref CDC_DSUBTYPE_CSInterface_Union.
    uint8_t                 MasterInterfaceNumber; // Interface number of the CDC Control interface.
    uint8_t                 SlaveInterfaceNumber;  // Interface number of the CDC Data interface.
} PACKED USB_CDC_Descriptor_FunctionalUnion_t;

/**
 *  Type define for a CDC Line Encoding structure, used to hold the various encoding parameters for a virtual
 *  serial port.
 */
typedef struct {
    uint32_t BaudRateBPS; // Baud rate of the virtual serial port, in bits per second.
    uint8_t  CharFormat;  // Character format of the virtual serial port, a value from the CDC_LineEncodingFormats_t enum.
    uint8_t  ParityType;  // Parity setting of the virtual serial port, a value from the CDC_LineEncodingParity_t enum.
    uint8_t  DataBits;    // Bits of data per character of the virtual serial port.
} PACKED CDC_LineEncoding_t;