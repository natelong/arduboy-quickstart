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
 * Common USB Controller definitions for all architectures.
 */

#pragma once

#include "../../../Common/Common.h"
#include "USBMode.h"

/**
 * Endpoint address direction masks for a Host to Device endpoint.
 * These may be ORed with the index of the address within a device to obtain the full endpoint address.
 */
#define ENDPOINT_DIR_OUT 0x00 // Mask for OUT direction
#define ENDPOINT_DIR_IN  0x80 // Mask for IN direction

#define EP_TYPE_CONTROL     0x00 // Mask for a CONTROL type endpoint or pipe.
#define EP_TYPE_ISOCHRONOUS 0x01 // Mask for an ISOCHRONOUS type endpoint or pipe.
#define EP_TYPE_BULK        0x02 // Mask for a BULK type endpoint or pipe.
#define EP_TYPE_INTERRUPT   0x03 // Mask for an INTERRUPT type endpoint or pipe.

#include "AVR8/USBController_AVR8.h"
