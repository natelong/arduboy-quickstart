/**
 *  Endpoint data stream transmission and reception management.
 */

#pragma once

#include "../../ab_common.h"
#include "USBTask.h"

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
