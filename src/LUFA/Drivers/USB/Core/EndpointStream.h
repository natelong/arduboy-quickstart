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
 *  Endpoint data stream transmission and reception management.
 */

#pragma once

#include "../../../Common/Common.h"
#include "USBMode.h"
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

/** Writes the given number of bytes to the endpoint from the given buffer in little endian,
 *  sending full packets to the host as needed. The last packet filled is not automatically sent;
 *  the user is responsible for manually sending the last written packet to the host via the
 *  \ref Endpoint_ClearIN() macro.
 *
 *  If the BytesProcessed parameter is \c NULL, the entire stream transfer is attempted at once,
 *  failing or succeeding as a single unit. If the BytesProcessed parameter points to a valid
 *  storage location, the transfer will instead be performed as a series of chunks. Each time
 *  the endpoint bank becomes full while there is still data to process (and after the current
 *  packet transmission has been initiated) the BytesProcessed location will be updated with the
 *  total number of bytes processed in the stream, and the function will exit with an error code of
 *  \ref ENDPOINT_RWSTREAM_IncompleteTransfer. This allows for any abort checking to be performed
 *  in the user code - to continue the transfer, call the function again with identical parameters
 *  and it will resume until the BytesProcessed value reaches the total transfer length.
 *
 *  <b>Single Stream Transfer Example:</b>
 *  \code
 *  uint8_t DataStream[512];
 *  uint8_t ErrorCode;
 *
 *  if ((ErrorCode = Endpoint_Write_Stream_LE(DataStream, sizeof(DataStream),
 *                                            NULL)) != ENDPOINT_RWSTREAM_NoError)
 *  {
 *       // Stream failed to complete - check ErrorCode here
 *  }
 *  \endcode
 *
 *  <b>Partial Stream Transfers Example:</b>
 *  \code
 *  uint8_t  DataStream[512];
 *  uint8_t  ErrorCode;
 *  uint16_t BytesProcessed;
 *
 *  BytesProcessed = 0;
 *  while ((ErrorCode = Endpoint_Write_Stream_LE(DataStream, sizeof(DataStream),
 *                                               &BytesProcessed)) == ENDPOINT_RWSTREAM_IncompleteTransfer)
 *  {
 *      // Stream not yet complete - do other actions here, abort if required
 *  }
 *
 *  if (ErrorCode != ENDPOINT_RWSTREAM_NoError)
 *  {
 *      // Stream failed to complete - check ErrorCode here
 *  }
 *  \endcode
 *
 *  \note This routine should not be used on CONTROL type endpoints.
 *
 *  \param[in] Buffer          Pointer to the source data buffer to read from.
 *  \param[in] Length          Number of bytes to read for the currently selected endpoint into the buffer.
 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes processed in the current
 *                             transaction should be updated, \c NULL if the entire stream should be written at once.
 *
 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
 */
uint8_t Endpoint_Write_Stream_LE(const void* const Buffer,
                                 uint16_t Length,
                                 uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1);

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
uint8_t Endpoint_Write_Control_Stream_LE(const void* const Buffer,
                                         uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

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
uint8_t Endpoint_Read_Control_Stream_LE(void* const Buffer,
                                        uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);
