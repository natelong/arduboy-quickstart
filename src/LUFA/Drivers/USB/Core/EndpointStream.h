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

#include "AVR8/EndpointStream_AVR8.h"
