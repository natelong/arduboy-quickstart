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

/** Endpoint number mask, for masking against endpoint addresses to retrieve the endpoint's
 *  numerical address in the device.
 */
#define ENDPOINT_EPNUM_MASK 0x07

/** Endpoint address for the default control endpoint, which always resides in address 0. This is
 *  defined for convenience to give more readable code when used with the endpoint macros.
 */
#define ENDPOINT_CONTROLEP 0

#include "AVR8/Endpoint_AVR8.h"
