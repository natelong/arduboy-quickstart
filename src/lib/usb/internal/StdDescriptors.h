/**
 *  Common standard USB Descriptor definitions for all architectures.
 */

#pragma once

#include "../../ab_common.h"
#include "Events.h"


/** Indicates that a given descriptor does not exist in the device. This can be used inside descriptors
 *  for string descriptor indexes, or may be use as a return value for GetDescriptor when the specified
 *  descriptor does not exist.
 */
#define NO_DESCRIPTOR 0

/** Macro to calculate the power value for the configuration descriptor, from a given number of milliamperes.
 *
 *  \param[in] mA  Maximum number of milliamps the device consumes when the given configuration is selected.
 */
#define USB_CONFIG_POWER_MA(mA) ((mA) >> 1)

/** Macro to calculate the Unicode length of a string with a given number of Unicode characters.
 *  Should be used in string descriptor's headers for giving the string descriptor's byte length.
 *
 *  \param[in] UnicodeChars  Number of Unicode characters in the string text.
 */
#define USB_STRING_LEN(UnicodeChars) (sizeof(USB_Descriptor_Header_t) + ((UnicodeChars) << 1))

/** Macro to encode a given four digit floating point version number (e.g. 01.23) into Binary Coded
 *  Decimal format for descriptor fields requiring BCD encoding, such as the USB version number in the
 *  standard device descriptor.
 *
 *  \note This value is automatically converted into Little Endian, suitable for direct use inside device
 *        descriptors on all architectures without endianness conversion macros.
 *
 *  \param[in]  x  Version number to encode as a 16-bit little-endian number, as a floating point number.
 */
#define VERSION_BCD(x) (((VERSION_TENS(x) << 4) | VERSION_ONES(x)) << 8) | \
                       ((VERSION_TENTHS(x) << 4) | VERSION_HUNDREDTHS(x))

/** String language ID for the English language. Should be used in \ref USB_Descriptor_String_t descriptors
 *  to indicate that the English language is supported by the device in its string descriptors.
 */
#define LANGUAGE_ID_ENG 0x0409



/** Can be masked with other configuration descriptor attributes for a USB_Descriptor_Configuration_Header_t
 *  descriptor's ConfigAttributes value to indicate from where the specified configuration can draw its power.
 */
#define USB_CONFIG_ATTR_BUSPOWERED   0x80 // host's VBUS
#define USB_CONFIG_ATTR_SELFPOWERED  0x40 // device's own power
#define USB_CONFIG_ATTR_REMOTEWAKEUP 0x20 // supports remote wakeup

/** Can be masked with other endpoint descriptor attributes for a USB_Descriptor_Endpoint_t descriptor's
 *  Attributes value to indicate how the specific endpoint behaves
 */
#define ENDPOINT_ATTR_NO_SYNC  (0 << 2) // not synchronized.
#define ENDPOINT_ATTR_ASYNC    (1 << 2) // asynchronous
#define ENDPOINT_ATTR_ADAPTIVE (2 << 2) // adaptive
#define ENDPOINT_ATTR_SYNC     (3 << 2) // synchronized

/** Can be masked with other endpoint descriptor attributes for a USB_Descriptor_Endpoint_t descriptor's
 *  Attributes value to indicate what the specified endpoint is used for.
 */
#define ENDPOINT_USAGE_DATA               (0 << 4) // data transfers
#define ENDPOINT_USAGE_FEEDBACK           (1 << 4) // feedback
#define ENDPOINT_USAGE_IMPLICIT_FEEDBACK  (2 << 4) // implicit feedback

/** Enum for the possible standard descriptor types, as given in each descriptor's header. */
enum USB_DescriptorTypes_t {
    DTYPE_Device                    = 0x01, /**< Indicates that the descriptor is a device descriptor. */
    DTYPE_Configuration             = 0x02, /**< Indicates that the descriptor is a configuration descriptor. */
    DTYPE_String                    = 0x03, /**< Indicates that the descriptor is a string descriptor. */
    DTYPE_Interface                 = 0x04, /**< Indicates that the descriptor is an interface descriptor. */
    DTYPE_Endpoint                  = 0x05, /**< Indicates that the descriptor is an endpoint descriptor. */
    DTYPE_DeviceQualifier           = 0x06, /**< Indicates that the descriptor is a device qualifier descriptor. */
    DTYPE_Other                     = 0x07, /**< Indicates that the descriptor is of other type. */
    DTYPE_InterfacePower            = 0x08, /**< Indicates that the descriptor is an interface power descriptor. */
    DTYPE_InterfaceAssociation      = 0x0B, /**< Indicates that the descriptor is an interface association descriptor. */
    DTYPE_CSInterface               = 0x24, /**< Indicates that the descriptor is a class specific interface descriptor. */
    DTYPE_CSEndpoint                = 0x25, /**< Indicates that the descriptor is a class specific endpoint descriptor. */
};

/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors. */
enum USB_Descriptor_ClassSubclassProtocol_t {
    USB_CSCP_NoDeviceClass          = 0x00, // device does not belong to a particular class at the device level.
    USB_CSCP_NoDeviceSubclass       = 0x00, // device does not belong to a particular subclass at the device level.
    USB_CSCP_NoDeviceProtocol       = 0x00, // device does not belong to a particular protocol at the device level.
    USB_CSCP_VendorSpecificClass    = 0xFF, // device/interface belongs to a vendor specific class.
    USB_CSCP_VendorSpecificSubclass = 0xFF, // device/interface belongs to a vendor specific subclass.
    USB_CSCP_VendorSpecificProtocol = 0xFF, // device/interface belongs to a vendor specific protocol.
    USB_CSCP_IADDeviceClass         = 0xEF, // device belongs to the Interface Association Descriptor class.
    USB_CSCP_IADDeviceSubclass      = 0x02, // device belongs to the Interface Association Descriptor subclass.
    USB_CSCP_IADDeviceProtocol      = 0x01, // device belongs to the Interface Association Descriptor protocol.
};

/**
 *  Type define for all descriptors' standard header, indicating the descriptor's length and type. This structure
 *  uses LUFA-specific element names to make each element's purpose clearer.
 */
typedef struct {
    uint8_t Size; // Size of the descriptor, in bytes.
    uint8_t Type; // Type of the descriptor, either in USB_DescriptorTypes_t or given by the specific class.
} PACKED USB_Descriptor_Header_t;

/**
 *  Type define for a standard Device Descriptor. This structure uses LUFA-specific element names to make each
 *  element's purpose clearer.
 */
typedef struct {
    USB_Descriptor_Header_t Header;  // Descriptor header, including type and size.
    uint16_t USBSpecification;       // BCD of the supported USB specification.
    uint8_t  Class;                  // USB device class.
    uint8_t  SubClass;               // USB device subclass.
    uint8_t  Protocol;               // USB device protocol.
    uint8_t  Endpoint0Size;          // Size of the control (address 0) endpoint's bank in bytes.
    uint16_t VendorID;               // Vendor ID for the USB product.
    uint16_t ProductID;              // Unique product ID for the USB product.
    uint16_t ReleaseNumber;          // Product release (version) number.
    uint8_t  ManufacturerStrIndex;   // String index for the manufacturer's name. The host will request this string via a separate
                                     // control request for the string descriptor. If no string supplied, use NO_DESCRIPTOR.
    uint8_t  ProductStrIndex;        // String index for product name/details.
    uint8_t  SerialNumStrIndex;      // String index for product's globally unique hex serial number in uppercase Unicode ASCII.
    uint8_t  NumberOfConfigurations; // Total number of configurations supported by the device.
} PACKED USB_Descriptor_Device_t;

/**
 *  Type define for a standard Device Qualifier Descriptor. This structure uses LUFA-specific element names
 *  to make each element's purpose clearer.
 */
typedef struct {
    USB_Descriptor_Header_t Header;  // Descriptor header, including type and size.
    uint16_t USBSpecification;       // BCD of the supported USB specification.
    uint8_t  Class;                  // USB device class.
    uint8_t  SubClass;               // USB device subclass.
    uint8_t  Protocol;               // USB device protocol.
    uint8_t  Endpoint0Size;          // Size of the control (address 0) endpoint's bank in bytes.
    uint8_t  NumberOfConfigurations; // Total number of configurations supported by the device.
    uint8_t  Reserved;               // Reserved for future use, must be 0.
} PACKED USB_Descriptor_DeviceQualifier_t;

/**
 *  Type define for a standard Configuration Descriptor header. This structure uses LUFA-specific element names
 *  to make each element's purpose clearer.
 */
typedef struct {
    USB_Descriptor_Header_t Header;  // Descriptor header, including type and size.
    uint16_t TotalConfigurationSize; // Size of the configuration descriptor header, and all sub descriptors inside the configuration.
    uint8_t  TotalInterfaces;        // Total number of interfaces in the configuration.
    uint8_t  ConfigurationNumber;    // Configuration index of the current configuration.
    uint8_t  ConfigurationStrIndex;  // Index of a string descriptor describing the configuration.
    uint8_t  ConfigAttributes;       // Configuration attributes, comprised of a mask of zero or more USB_CONFIG_ATTR_* masks.
    uint8_t  MaxPowerConsumption;    // Max power consumption of in the current configuration, calculated by USB_CONFIG_POWER_MA() macro.
} PACKED USB_Descriptor_Configuration_Header_t;

/**
 *  Type define for a standard Interface Descriptor. This structure uses LUFA-specific element names
 *  to make each element's purpose clearer.
 */
typedef struct {
    USB_Descriptor_Header_t Header; // Descriptor header, including type and size.
    uint8_t InterfaceNumber;        // Index of the interface in the current configuration.
    uint8_t AlternateSetting;       // Alternate setting for interface number. Can have alternate settings with different endpoint
                                    // configurations, which can be selected by the host.
    uint8_t TotalEndpoints;         // Total number of endpoints in the interface.
    uint8_t Class;                  // Interface class ID.
    uint8_t SubClass;               // Interface subclass ID.
    uint8_t Protocol;               // Interface protocol ID.
    uint8_t InterfaceStrIndex;      // Index of the string descriptor describing the interface.
} PACKED USB_Descriptor_Interface_t;

/**
 *  Type define for a standard Interface Association Descriptor. This structure uses LUFA-specific element names
 *  to make each element's purpose clearer.
 *
 *  This descriptor has been added as a supplement to the USB2.0 standard, in the ECN located at
 *  <a>http://www.usb.org/developers/docs/InterfaceAssociationDescriptor_ecn.pdf</a>. It allows composite
 *  devices with multiple interfaces related to the same function to have the multiple interfaces bound
 *  together at the point of enumeration, loading one generic driver for all the interfaces in the single
 *  function. Read the ECN for more information.
 */
typedef struct {
    USB_Descriptor_Header_t Header; // Descriptor header, including type and size.
    uint8_t FirstInterfaceIndex;    // Index of the first associated interface.
    uint8_t TotalInterfaces;        // Total number of associated interfaces.
    uint8_t Class;                  // Interface class ID.
    uint8_t SubClass;               // Interface subclass ID.
    uint8_t Protocol;               // Interface protocol ID.
    uint8_t IADStrIndex;            // Index of the string descriptor describing the interface association.
} PACKED USB_Descriptor_Interface_Association_t;

/**
 *  Type define for a standard Endpoint Descriptor. This structure uses LUFA-specific element names
 *  to make each element's purpose clearer.
 */
typedef struct {
    USB_Descriptor_Header_t Header; // Descriptor header, including type and size.
    uint8_t  EndpointAddress;       // Logical address of endpoint within device for current configuration including direction mask.
    uint8_t  Attributes;            // Endpoint attributes: mask of endpoint type (EP_TYPE_*) and attributes (ENDPOINT_ATTR_*) masks.
    uint16_t EndpointSize;          // Size of the endpoint bank in bytes: max packet size that the endpoint can receive at a time.
    uint8_t  PollingIntervalMS;     // Polling interval in milliseconds for the endpoint if it is an INTERRUPT or ISOCHRONOUS type.
} PACKED USB_Descriptor_Endpoint_t;

/**
 *  Type define for a standard string descriptor. Unlike other standard descriptors, the length
 *  of the descriptor for placement in the descriptor header must be determined by the \ref USB_STRING_LEN()
 *  macro rather than by the size of the descriptor structure, as the length is not fixed.
 *
 *  This structure should also be used for string index 0, which contains the supported language IDs for
 *  the device as an array.
 */
typedef struct {
    USB_Descriptor_Header_t Header; // Descriptor header, including type and size.
    wchar_t  UnicodeString[];       // String data as unicode characters.
} PACKED USB_Descriptor_String_t;

#define VERSION_TENS(x)       (int)((x) / 10)
#define VERSION_ONES(x)       (int)((x) - (10 * VERSION_TENS(x)))
#define VERSION_TENTHS(x)     (int)(((x) - (int)(x)) * 10)
#define VERSION_HUNDREDTHS(x) (int)((((x) - (int)(x)) * 100) - (10 * VERSION_TENTHS(x)))
