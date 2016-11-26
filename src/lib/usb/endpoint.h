/**
 *  USB Endpoint definitions for all architectures.
 */

#pragma once

#include "../ab_common.h"
#include "interrupt.h"

#define EP_DIR_OUT 0x00 // Mask for OUT direction
#define EP_DIR_IN  0x80 // Mask for IN direction

#define EP_TYPE_CONTROL     0x00 // Mask for a CONTROL type endpoint or pipe.
#define EP_TYPE_ISOCHRONOUS 0x01 // Mask for an ISOCHRONOUS type endpoint or pipe.
#define EP_TYPE_BULK        0x02 // Mask for a BULK type endpoint or pipe.
#define EP_TYPE_INTERRUPT   0x03 // Mask for an INTERRUPT type endpoint or pipe.

/** Endpoint number mask, for masking against endpoint addresses to retrieve the endpoint's
 *  numerical address in the device.
 */
#define ENDPOINT_EPNUM_MASK 0x07

/** Endpoint address for the default control endpoint, which always resides in address 0. This is
 *  defined for convenience to give more readable code when used with the endpoint macros.
 */
#define ENDPOINT_CONTROLEP 0

/** Global indicating the maximum packet size of the default control endpoint located at address
 *  0 in the device. Used to be controlled by FIXED_CONTROL_ENDPOINT_SIZE
 */
#define USB_Device_ControlEndpointSize 8

/** Completes the status stage of a control transfer on a CONTROL type endpoint automatically,
 *  with respect to the data direction. This is a convenience function which can be used to
 *  simplify user control request handling.
 */
void Endpoint_ClearStatusStage(void);

/** Spin-loops until the currently selected non-control endpoint is ready for the next packet of data
 *  to be read or written to it.
 *  \return A value from the \ref Endpoint_WaitUntilReady_ErrorCodes_t enum.
 */
uint8_t Endpoint_WaitUntilReady(void);

void Endpoint_ClearEndpoints(void);

/** Mask for the bank mode selection for the \ref Endpoint_ConfigureEndpoint() macro. This indicates
 *  that the endpoint should have one single bank, which requires less USB FIFO memory but results
 *  in slower transfers as only one USB device (the AVR or the host) can access the endpoint's
 *  bank at the one time.
 */
#define ENDPOINT_BANK_SINGLE (0 << EPBK0)

#define ENDPOINT_TOTAL_ENDPOINTS 7

// Enum for the possible error return codes of the \ref Endpoint_WaitUntilReady() function.
enum Endpoint_WaitUntilReady_ErrorCodes_t {
    ENDPOINT_READYWAIT_NoError            = 0, // Endpoint is ready for next packet, no error
    ENDPOINT_READYWAIT_EndpointStalled    = 1, // The endpoint was stalled during the stream transfer by the host or device
    ENDPOINT_READYWAIT_DeviceDisconnected = 2, // Device was disconnected from the host while waiting for the endpoint to become ready
    ENDPOINT_READYWAIT_BusSuspended       = 3, // USB bus suspended by host. No endpoint traffic can occur until the bus has resumed
    ENDPOINT_READYWAIT_Timeout            = 4, // Host failed to accept or send next packet within timeout set by USB_STREAM_TIMEOUT_MS macro
};

/** Selects the given endpoint number. If the address from the device descriptors is used, the
 *  value should be masked with the \ref ENDPOINT_EPNUM_MASK constant to extract only the endpoint
 *  number (and discarding the endpoint direction bit).
 *
 *  Any endpoint operations which do not require the endpoint number to be indicated will operate on
 *  the currently selected endpoint.
 *
 *  \param[in] EndpointNumber Endpoint number to select.
 */
static INLINE void Endpoint_SelectEndpoint(const uint8_t EndpointNumber) {
    UENUM = EndpointNumber;
}

/** Enables the currently selected endpoint so that data can be sent and received through it to
 *  and from a host.
 */
static INLINE void Endpoint_EnableEndpoint(void) {
    UECONX |= (1 << EPEN);
}

// Indicates the number of bytes currently stored in the current endpoint's selected bank.
static INLINE uint16_t Endpoint_BytesInEndpoint(void) {
    return (((uint16_t)UEBCHX << 8) | UEBCLX);
}

/** Get the endpoint address of the currently selected endpoint. This is typically used to save
 *  the currently selected endpoint number so that it can be restored after another endpoint has
 *  been manipulated.
 *
 *  \return Index of the currently selected endpoint.
 */
static INLINE uint8_t Endpoint_GetCurrentEndpoint(void) {
    return (UENUM & ENDPOINT_EPNUM_MASK);
}

/** Resets the endpoint bank FIFO. This clears all the endpoint banks and resets the USB controller's
 *  data In and Out pointers to the bank's contents.
 *
 *  \param[in] EndpointNumber Endpoint number whose FIFO buffers are to be reset.
 */
static INLINE void Endpoint_ResetEndpoint(const uint8_t EndpointNumber) {
    UERST = (1 << EndpointNumber);
    UERST = 0;
}

/** Disables the currently selected endpoint so that data cannot be sent and received through it
 *  to and from a host.
 */
static INLINE void Endpoint_DisableEndpoint(void) {
    UECONX &= ~(1 << EPEN);
}

/** Determines if the currently selected endpoint is enabled, but not necessarily configured.
 *
 * \return Boolean \c true if the currently selected endpoint is enabled, \c false otherwise.
 */
static INLINE bool Endpoint_IsEnabled(void) {
    return ((UECONX & (1 << EPEN)) ? true : false);
}

/** Determines if the currently selected endpoint is configured.
 *
 *  \return Boolean \c true if the currently selected endpoint has been configured, \c false otherwise.
 */
static INLINE bool Endpoint_IsConfigured(void) {
    return ((UESTA0X & (1 << CFGOK)) ? true : false);
}

/** Determines if the selected IN endpoint is ready for a new packet to be sent to the host.
 *
 *  \return Boolean \c true if the current endpoint is ready for an IN packet, \c false otherwise.
 */
static INLINE bool Endpoint_IsINReady(void) {
    return ((UEINTX & (1 << TXINI)) ? true : false);
}

/** Determines if the selected OUT endpoint has received new packet from the host.
 *
 *  \return Boolean \c true if current endpoint is has received an OUT packet, \c false otherwise.
 */
static INLINE bool Endpoint_IsOUTReceived(void) {
    return ((UEINTX & (1 << RXOUTI)) ? true : false);
}

/** Determines if the current CONTROL type endpoint has received a SETUP packet.
 *
 *  \return Boolean \c true if the selected endpoint has received a SETUP packet, \c false otherwise.
 */
static INLINE bool Endpoint_IsSETUPReceived(void) {
    return ((UEINTX & (1 << RXSTPI)) ? true : false);
}

/** Clears a received SETUP packet on the currently selected CONTROL type endpoint, freeing up the
 *  endpoint for the next packet.
 *
 *  \note This is not applicable for non CONTROL type endpoints.
 */
static INLINE void Endpoint_ClearSETUP(void) {
    UEINTX &= ~(1 << RXSTPI);
}

/** Sends an IN packet to the host on the currently selected endpoint, freeing up the endpoint for the
 *  next packet and switching to the alternative endpoint bank if double banked.
 */
static INLINE void Endpoint_ClearIN(void) {
    UEINTX &= ~((1 << TXINI) | (1 << FIFOCON));
}

/** Acknowledges an OUT packet to the host on the currently selected endpoint, freeing up the endpoint
 *  for the next packet and switching to the alternative endpoint bank if double banked.
 */
static INLINE void Endpoint_ClearOUT(void) {
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
static INLINE void Endpoint_StallTransaction(void) {
    UECONX |= (1 << STALLRQ);
}

/** Clears the STALL condition on the currently selected endpoint.
 */
static INLINE void Endpoint_ClearStall(void) {
    UECONX |= (1 << STALLRQC);
}

/** Determines if the currently selected endpoint is stalled, false otherwise.
 *
 *  \return Boolean \c true if the currently selected endpoint is stalled, \c false otherwise.
 */
static INLINE bool Endpoint_IsStalled(void) {
    return ((UECONX & (1 << STALLRQ)) ? true : false);
}

/** Resets the data toggle of the currently selected endpoint. */
static INLINE void Endpoint_ResetDataToggle(void) {
    UECONX |= (1 << RSTDT);
}

/** Determines the currently selected endpoint's direction.
 *
 *  \return The currently selected endpoint's direction, as a \c ENDPOINT_DIR_* mask.
 */
static INLINE uint8_t Endpoint_GetEndpointDirection(void) {
    return (UECFG0X & (1 << EPDIR)) ? EP_DIR_IN : EP_DIR_OUT;
}

/** Sets the direction of the currently selected endpoint.
 *
 *  \param[in] DirectionMask  New endpoint direction, as a \c ENDPOINT_DIR_* mask.
 */
static INLINE void Endpoint_SetEndpointDirection(const uint8_t DirectionMask) {
    UECFG0X = ((UECFG0X & ~(1 << EPDIR)) | (DirectionMask ? (1 << EPDIR) : 0));
}

/** Reads one byte from the currently selected endpoint's bank, for OUT direction endpoints.
 *
 *  \return Next byte in the currently selected endpoint's FIFO buffer.
 */
static INLINE uint8_t Endpoint_Read_8(void) {
    return UEDATX;
}

/** Writes one byte to the currently selected endpoint's bank, for IN direction endpoints.
 *
 *  \param[in] Data  Data to write into the the currently selected endpoint's FIFO buffer.
 */
static INLINE void Endpoint_Write_8(const uint8_t Data) {
    UEDATX = Data;
}

/** Writes two bytes to the currently selected endpoint's bank in little endian format, for IN
 *  direction endpoints.
 *
 *  \param[in] Data  Data to write to the currently selected endpoint's FIFO buffer.
 */
static INLINE void Endpoint_Write_16(const uint16_t Data) {
    UEDATX = (Data & 0xFF);
    UEDATX = (Data >> 8);
}

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
 *  \param[in] Direction  Endpoint data direction, either \ref EP_DIR_OUT or \ref EP_DIR_IN.
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
static INLINE bool Endpoint_ConfigureEndpoint(const uint8_t Number, const uint8_t Type, const uint8_t Direction, const uint16_t Size, const uint8_t Banks) {

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

    { // Endpoint_ConfigureEndpoint_Prv (const uint8_t Number, const uint8_t UECFG0XData, const uint8_t UECFG1XData)
        Endpoint_SelectEndpoint(Number);
        Endpoint_EnableEndpoint();

        UECFG1X = 0;
        UECFG0X = ((Type << EPTYPE0) | (Direction ? (1 << EPDIR) : 0));
        UECFG1X = ((1 << ALLOC) | Banks | epSizeMask);

        return Endpoint_IsConfigured();
    }
}


/** Enum for possible error return codes of Endpoint_*_Stream_* functions. */
enum Endpoint_Stream_RW_ErrorCodes_t {
    ENDPOINT_RWSTREAM_NoError            = 0, // Command completed successfully, no error.
    ENDPOINT_RWSTREAM_EndpointStalled    = 1, // Endpoint was stalled during stream transfer by host or device.
    ENDPOINT_RWSTREAM_DeviceDisconnected = 2, // Device was disconnected from host during transfer.
    ENDPOINT_RWSTREAM_BusSuspended       = 3, // The USB bus was suspended by host and no traffic can occur until bus has resumed.
    ENDPOINT_RWSTREAM_Timeout            = 4, // Host failed to accept or send next packet within timeout period from USB_STREAM_TIMEOUT_MS
    ENDPOINT_RWSTREAM_IncompleteTransfer = 5, // Endpoint bank became full or empty before complete contents of current stream could be
                                              // transferred. Endpoint stream function should be called again to process next chunk of data.
};

/** Enum for possible error return codes of Endpoint_*_Control_Stream_* functions. */
enum Endpoint_ControlStream_RW_ErrorCodes_t {
    ENDPOINT_RWCSTREAM_NoError            = 0, // Command completed successfully, no error.
    ENDPOINT_RWCSTREAM_HostAborted        = 1, // The aborted transfer prematurely.
    ENDPOINT_RWCSTREAM_DeviceDisconnected = 2, // Device was disconnected from host during transfer.
    ENDPOINT_RWCSTREAM_BusSuspended       = 3, // USB bus has been suspended by host and no traffic can occur until bus has resumed.
};

/** Writes the given number of bytes to the CONTROL type endpoint from the given buffer in little endian,
 *  sending full packets to the host as needed. The host OUT acknowledgement is not automatically cleared
 *  in both failure and success states; the user is responsible for manually clearing the setup OUT to
 *  finalize the transfer via the \ref Endpoint_ClearOUT() macro.
 *
 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
 *        to clear the status stage when using this routine in a control transaction.
 *        \n\n
 *
 *  \note This routine should only be used on CONTROL type endpoints.
 *
 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
 *           together; i.e. the entire stream data must be read or written at the one time.
 *
 *  \param[in] Buffer  Pointer to the source data buffer to read from.
 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
 *
 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
 */
uint8_t Endpoint_Write_Control_Stream(const void* const Buffer, uint16_t Length);

/** Reads the given number of bytes from the CONTROL endpoint from the given buffer in little endian,
 *  discarding fully read packets from the host as needed. The device IN acknowledgement is not
 *  automatically sent after success or failure states; the user is responsible for manually sending the
 *  setup IN to finalize the transfer via the \ref Endpoint_ClearIN() macro.
 *
 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
 *        to clear the status stage when using this routine in a control transaction.
 *        \n\n
 *
 *  \note This routine should only be used on CONTROL type endpoints.
 *
 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
 *           together; i.e. the entire stream data must be read or written at the one time.
 *
 *  \param[out] Buffer  Pointer to the destination data buffer to write to.
 *  \param[in]  Length  Number of bytes to send via the currently selected endpoint.
 *
 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
 */
uint8_t Endpoint_Read_Control_Stream(void* const Buffer, uint16_t Length);