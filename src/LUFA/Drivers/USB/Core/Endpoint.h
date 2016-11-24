// name
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
 *  USB Endpoint definitions for all architectures.
 */

#pragma once

#include "../../../Common/Common.h"
#include "USBMode.h"
#include "USBTask.h"
#include "USBInterrupt.h"

/** Endpoint number mask, for masking against endpoint addresses to retrieve the endpoint's
 *  numerical address in the device.
 */
#define ENDPOINT_EPNUM_MASK 0x07

/** Endpoint address for the default control endpoint, which always resides in address 0. This is
 *  defined for convenience to give more readable code when used with the endpoint macros.
 */
#define ENDPOINT_CONTROLEP 0

#define _ENDPOINT_GET_MAXSIZE(EPIndex)         _ENDPOINT_GET_MAXSIZE2(ENDPOINT_DETAILS_EP ## EPIndex)
#define _ENDPOINT_GET_MAXSIZE2(EPDetails)      _ENDPOINT_GET_MAXSIZE3(EPDetails)
#define _ENDPOINT_GET_MAXSIZE3(MaxSize, Banks) (MaxSize)

#define _ENDPOINT_GET_BANKS(EPIndex)           _ENDPOINT_GET_BANKS2(ENDPOINT_DETAILS_EP ## EPIndex)
#define _ENDPOINT_GET_BANKS2(EPDetails)        _ENDPOINT_GET_BANKS3(EPDetails)
#define _ENDPOINT_GET_BANKS3(MaxSize, Banks)   (Banks)

#define ENDPOINT_DETAILS_MAXEP 7
#define ENDPOINT_DETAILS_EP0   64,  1
#define ENDPOINT_DETAILS_EP1   256, 2
#define ENDPOINT_DETAILS_EP2   64,  2
#define ENDPOINT_DETAILS_EP3   64,  2
#define ENDPOINT_DETAILS_EP4   64,  2
#define ENDPOINT_DETAILS_EP5   64,  2
#define ENDPOINT_DETAILS_EP6   64,  2

/** Global indicating the maximum packet size of the default control endpoint located at address
 *  0 in the device. This value is set to the value indicated in the device descriptor in the user
 *  project once the USB interface is initialized into device mode.
 *
 *  If space is an issue, it is possible to fix this to a static value by defining the control
 *  endpoint size in the \c FIXED_CONTROL_ENDPOINT_SIZE token passed to the compiler in the makefile
 *  via the -D switch. When a fixed control endpoint size is used, the size is no longer dynamically
 *  read from the descriptors at runtime and instead fixed to the given value. When used, it is
 *  important that the descriptor control endpoint size value matches the size given as the
 *  \c FIXED_CONTROL_ENDPOINT_SIZE token - it is recommended that the \c FIXED_CONTROL_ENDPOINT_SIZE token
 *  be used in the device descriptors to ensure this.
 *
 *  \note This variable should be treated as read-only in the user application, and never manually
 *        changed in value.
 */
#define USB_Device_ControlEndpointSize FIXED_CONTROL_ENDPOINT_SIZE

/** Completes the status stage of a control transfer on a CONTROL type endpoint automatically,
 *  with respect to the data direction. This is a convenience function which can be used to
 *  simplify user control request handling.
 */
void Endpoint_ClearStatusStage(void);

/** Spin-loops until the currently selected non-control endpoint is ready for the next packet of data
 *  to be read or written to it.
 *
 *  \note This routine should not be called on CONTROL type endpoints.
 *
 *  \return A value from the \ref Endpoint_WaitUntilReady_ErrorCodes_t enum.
 */
uint8_t Endpoint_WaitUntilReady(void);

void Endpoint_ClearEndpoints(void);

bool Endpoint_ConfigureEndpoint_Prv(const uint8_t Number, const uint8_t UECFG0XData, const uint8_t UECFG1XData);

/** Mask for the bank mode selection for the \ref Endpoint_ConfigureEndpoint() macro. This indicates
 *  that the endpoint should have one single bank, which requires less USB FIFO memory but results
 *  in slower transfers as only one USB device (the AVR or the host) can access the endpoint's
 *  bank at the one time.
 */
#define ENDPOINT_BANK_SINGLE (0 << EPBK0)

/** Mask for the bank mode selection for the \ref Endpoint_ConfigureEndpoint() macro. This indicates
 *  that the endpoint should have two banks, which requires more USB FIFO memory but results
 *  in faster transfers as one USB device (the AVR or the host) can access one bank while the other
 *  accesses the second bank.
 */
#define ENDPOINT_BANK_DOUBLE (1 << EPBK0)

// Retrieves the maximum bank size in bytes of a given endpoint.
#define ENDPOINT_MAX_SIZE(EPIndex) _ENDPOINT_GET_MAXSIZE(EPIndex)

// Retrieves the total number of banks supported by the given endpoint.
#define ENDPOINT_BANKS_SUPPORTED(EPIndex) _ENDPOINT_GET_BANKS(EPIndex)

#define ENDPOINT_TOTAL_ENDPOINTS ENDPOINT_DETAILS_MAXEP

// Enum for the possible error return codes of the \ref Endpoint_WaitUntilReady() function.
enum Endpoint_WaitUntilReady_ErrorCodes_t {
    ENDPOINT_READYWAIT_NoError            = 0, // Endpoint is ready for next packet, no error
    ENDPOINT_READYWAIT_EndpointStalled    = 1, // The endpoint was stalled during the stream transfer by the host or device
    ENDPOINT_READYWAIT_DeviceDisconnected = 2, // Device was disconnected from the host while waiting for the endpoint to become ready
    ENDPOINT_READYWAIT_BusSuspended       = 3, // USB bus suspended by host. No endpoint traffic can occur until the bus has resumed
    ENDPOINT_READYWAIT_Timeout            = 4, // Host failed to accept or send next packet within timeout set by USB_STREAM_TIMEOUT_MS macro
};

/** Configures the specified endpoint number with the given endpoint type, direction, bank size
 *  and banking mode. Once configured, the endpoint may be read from or written to, depending
 *  on its direction.
 *
 *  \param[in] Number     Endpoint number to configure. This must be more than 0 and less than
 *                        \ref ENDPOINT_TOTAL_ENDPOINTS.
 *
 *  \param[in] Type       Type of endpoint to configure, a \c EP_TYPE_* mask. Not all endpoint types
 *                        are available on Low Speed USB devices - refer to the USB 2.0 specification.
 *
 *  \param[in] Direction  Endpoint data direction, either \ref ENDPOINT_DIR_OUT or \ref ENDPOINT_DIR_IN.
 *                        All endpoints (except Control type) are unidirectional - data may only be read
 *                        from or written to the endpoint bank based on its direction, not both.
 *
 *  \param[in] Size       Size of the endpoint's bank, where packets are stored before they are transmitted
 *                        to the USB host, or after they have been received from the USB host (depending on
 *                        the endpoint's data direction). The bank size must indicate the maximum packet size
 *                        that the endpoint can handle.
 *
 *  \param[in] Banks      Number of banks to use for the endpoint being configured, an \c ENDPOINT_BANK_* mask.
 *                        More banks uses more USB DPRAM, but offers better performance. Isochronous type
 *                        endpoints <b>must</b> have at least two banks.
 *
 *  \note When the \c ORDERED_EP_CONFIG compile time option is used, Endpoints <b>must</b> be configured in
 *        ascending order, or bank corruption will occur.
 *        \n\n
 *
 *  \note Different endpoints may have different maximum packet sizes based on the endpoint's index - refer to
 *        the chosen microcontroller model's datasheet to determine the maximum bank size for each endpoint.
 *        \n\n
 *
 *  \note The default control endpoint should not be manually configured by the user application, as
 *        it is automatically configured by the library internally.
 *        \n\n
 *
 *  \note This routine will automatically select the specified endpoint upon success. Upon failure, the endpoint
 *        which failed to reconfigure correctly will be selected.
 *
 *  \return Boolean \c true if the configuration succeeded, \c false otherwise.
 */
static ALWAYS_INLINE bool Endpoint_ConfigureEndpoint(
        const uint8_t Number, const uint8_t Type, const uint8_t Direction, const uint16_t Size, const uint8_t Banks) {

    uint8_t epSizeMask;
    {
        uint8_t  MaskVal    = 0;
        uint16_t CheckBytes = 8;

        while (CheckBytes < Size) {
            MaskVal++;
            CheckBytes <<= 1;
        }

        epSizeMask = (MaskVal << EPSIZE0);
    }


    return Endpoint_ConfigureEndpoint_Prv(
        Number,
        ((Type << EPTYPE0) | (Direction ? (1 << EPDIR) : 0)),
        ((1 << ALLOC) | Banks | epSizeMask)
    );
}

// Indicates the number of bytes currently stored in the current endpoint's selected bank.
static ALWAYS_INLINE uint16_t Endpoint_BytesInEndpoint(void) {
    return (((uint16_t)UEBCHX << 8) | UEBCLX);
}

/** Get the endpoint address of the currently selected endpoint. This is typically used to save
 *  the currently selected endpoint number so that it can be restored after another endpoint has
 *  been manipulated.
 *
 *  \return Index of the currently selected endpoint.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT uint8_t Endpoint_GetCurrentEndpoint(void) {
    return (UENUM & ENDPOINT_EPNUM_MASK);
}

/** Selects the given endpoint number. If the address from the device descriptors is used, the
 *  value should be masked with the \ref ENDPOINT_EPNUM_MASK constant to extract only the endpoint
 *  number (and discarding the endpoint direction bit).
 *
 *  Any endpoint operations which do not require the endpoint number to be indicated will operate on
 *  the currently selected endpoint.
 *
 *  \param[in] EndpointNumber Endpoint number to select.
 */
static ALWAYS_INLINE void Endpoint_SelectEndpoint(const uint8_t EndpointNumber) {
    UENUM = EndpointNumber;
}

/** Resets the endpoint bank FIFO. This clears all the endpoint banks and resets the USB controller's
 *  data In and Out pointers to the bank's contents.
 *
 *  \param[in] EndpointNumber Endpoint number whose FIFO buffers are to be reset.
 */
static ALWAYS_INLINE void Endpoint_ResetEndpoint(const uint8_t EndpointNumber) {
    UERST = (1 << EndpointNumber);
    UERST = 0;
}

/** Enables the currently selected endpoint so that data can be sent and received through it to
 *  and from a host.
 *
 *  \note Endpoints must first be configured properly via \ref Endpoint_ConfigureEndpoint().
 */
static ALWAYS_INLINE void Endpoint_EnableEndpoint(void) {
    UECONX |= (1 << EPEN);
}

/** Disables the currently selected endpoint so that data cannot be sent and received through it
 *  to and from a host.
 */
static ALWAYS_INLINE void Endpoint_DisableEndpoint(void) {
    UECONX &= ~(1 << EPEN);
}

/** Determines if the currently selected endpoint is enabled, but not necessarily configured.
 *
 * \return Boolean \c true if the currently selected endpoint is enabled, \c false otherwise.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsEnabled(void) {
    return ((UECONX & (1 << EPEN)) ? true : false);
}

/** Determines if the currently selected endpoint may be read from (if data is waiting in the endpoint
 *  bank and the endpoint is an OUT direction, or if the bank is not yet full if the endpoint is an IN
 *  direction). This function will return false if an error has occurred in the endpoint, if the endpoint
 *  is an OUT direction and no packet (or an empty packet) has been received, or if the endpoint is an IN
 *  direction and the endpoint bank is full.
 *
 *  \return Boolean \c true if the currently selected endpoint may be read from or written to, depending
 *          on its direction.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsReadWriteAllowed(void) {
    return ((UEINTX & (1 << RWAL)) ? true : false);
}

/** Determines if the currently selected endpoint is configured.
 *
 *  \return Boolean \c true if the currently selected endpoint has been configured, \c false otherwise.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsConfigured(void) {
    return ((UESTA0X & (1 << CFGOK)) ? true : false);
}

/** Determines if the selected IN endpoint is ready for a new packet to be sent to the host.
 *
 *  \return Boolean \c true if the current endpoint is ready for an IN packet, \c false otherwise.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsINReady(void) {
    return ((UEINTX & (1 << TXINI)) ? true : false);
}

/** Determines if the selected OUT endpoint has received new packet from the host.
 *
 *  \return Boolean \c true if current endpoint is has received an OUT packet, \c false otherwise.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsOUTReceived(void) {
    return ((UEINTX & (1 << RXOUTI)) ? true : false);
}

/** Determines if the current CONTROL type endpoint has received a SETUP packet.
 *
 *  \return Boolean \c true if the selected endpoint has received a SETUP packet, \c false otherwise.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsSETUPReceived(void) {
    return ((UEINTX & (1 << RXSTPI)) ? true : false);
}

/** Clears a received SETUP packet on the currently selected CONTROL type endpoint, freeing up the
 *  endpoint for the next packet.
 *
 *  \note This is not applicable for non CONTROL type endpoints.
 */
static ALWAYS_INLINE void Endpoint_ClearSETUP(void) {
    UEINTX &= ~(1 << RXSTPI);
}

/** Sends an IN packet to the host on the currently selected endpoint, freeing up the endpoint for the
 *  next packet and switching to the alternative endpoint bank if double banked.
 */
static ALWAYS_INLINE void Endpoint_ClearIN(void) {
    UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));
}

/** Acknowledges an OUT packet to the host on the currently selected endpoint, freeing up the endpoint
 *  for the next packet and switching to the alternative endpoint bank if double banked.
 */
static ALWAYS_INLINE void Endpoint_ClearOUT(void) {
    UEINTX &= ~((1 << RXOUTI) | (1 << FIFOCON));
}

/** Stalls the current endpoint, indicating to the host that a logical problem occurred with the
 *  indicated endpoint and that the current transfer sequence should be aborted. This provides a
 *  way for devices to indicate invalid commands to the host so that the current transfer can be
 *  aborted and the host can begin its own recovery sequence.
 *
 *  The currently selected endpoint remains stalled until either the Endpoint_ClearStall() macro
 *  is called, or the host issues a CLEAR FEATURE request to the device for the currently selected
 *  endpoint.
 */
static ALWAYS_INLINE void Endpoint_StallTransaction(void) {
    UECONX |= (1 << STALLRQ);
}

/** Clears the STALL condition on the currently selected endpoint.
 */
static ALWAYS_INLINE void Endpoint_ClearStall(void) {
    UECONX |= (1 << STALLRQC);
}

/** Determines if the currently selected endpoint is stalled, false otherwise.
 *
 *  \return Boolean \c true if the currently selected endpoint is stalled, \c false otherwise.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT bool Endpoint_IsStalled(void) {
    return ((UECONX & (1 << STALLRQ)) ? true : false);
}

/** Resets the data toggle of the currently selected endpoint. */
static ALWAYS_INLINE void Endpoint_ResetDataToggle(void) {
    UECONX |= (1 << RSTDT);
}

/** Determines the currently selected endpoint's direction.
 *
 *  \return The currently selected endpoint's direction, as a \c ENDPOINT_DIR_* mask.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT uint8_t Endpoint_GetEndpointDirection(void) {
    return (UECFG0X & (1 << EPDIR)) ? ENDPOINT_DIR_IN : ENDPOINT_DIR_OUT;
}

/** Sets the direction of the currently selected endpoint.
 *
 *  \param[in] DirectionMask  New endpoint direction, as a \c ENDPOINT_DIR_* mask.
 */
static ALWAYS_INLINE void Endpoint_SetEndpointDirection(const uint8_t DirectionMask) {
    UECFG0X = ((UECFG0X & ~(1 << EPDIR)) | (DirectionMask ? (1 << EPDIR) : 0));
}

/** Reads one byte from the currently selected endpoint's bank, for OUT direction endpoints.
 *
 *  \return Next byte in the currently selected endpoint's FIFO buffer.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT uint8_t Endpoint_Read_8(void) {
    return UEDATX;
}

/** Writes one byte to the currently selected endpoint's bank, for IN direction endpoints.
 *
 *  \param[in] Data  Data to write into the the currently selected endpoint's FIFO buffer.
 */
static ALWAYS_INLINE void Endpoint_Write_8(const uint8_t Data) {
    UEDATX = Data;
}

/** Reads two bytes from the currently selected endpoint's bank in little endian format, for OUT
 *  direction endpoints.
 *
 *  \return Next two bytes in the currently selected endpoint's FIFO buffer.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT uint16_t Endpoint_Read_16_LE(void) {
    union {
        uint16_t Value;
        uint8_t  Bytes[2];
    } Data;

    Data.Bytes[0] = UEDATX;
    Data.Bytes[1] = UEDATX;

    return Data.Value;
}

/** Writes two bytes to the currently selected endpoint's bank in little endian format, for IN
 *  direction endpoints.
 *
 *  \param[in] Data  Data to write to the currently selected endpoint's FIFO buffer.
 */
static ALWAYS_INLINE void Endpoint_Write_16_LE(const uint16_t Data) {
    UEDATX = (Data & 0xFF);
    UEDATX = (Data >> 8);
}

/** Reads four bytes from the currently selected endpoint's bank in little endian format, for OUT
 *  direction endpoints.
 *
 *  \return Next four bytes in the currently selected endpoint's FIFO buffer.
 */
static ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT uint32_t Endpoint_Read_32_LE(void) {
    union {
        uint32_t Value;
        uint8_t  Bytes[4];
    } Data;

    Data.Bytes[0] = UEDATX;
    Data.Bytes[1] = UEDATX;
    Data.Bytes[2] = UEDATX;
    Data.Bytes[3] = UEDATX;

    return Data.Value;
}

/** Writes four bytes to the currently selected endpoint's bank in little endian format, for IN
 *  direction endpoints.
 *
 *  \param[in] Data  Data to write to the currently selected endpoint's FIFO buffer.
 */
static ALWAYS_INLINE void Endpoint_Write_32_LE(const uint32_t Data) {
    UEDATX = (Data &  0xFF);
    UEDATX = (Data >> 8);
    UEDATX = (Data >> 16);
    UEDATX = (Data >> 24);
}
