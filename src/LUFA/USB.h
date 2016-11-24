// nate
/*
 *  Master include file for the library USB functionality.
 *
 *  This file should be included in all user projects making use of the USB portions of the library, instead of
 *  the individual USB driver submodule headers.
 */

#pragma once

#include "internal/Common.h"
#include "internal/USBMode.h"

#include "internal/USBTask.h"
#include "internal/Events.h"
#include "internal/StdDescriptors.h"
#include "internal/ConfigDescriptor.h"
#include "internal/USBController.h"
#include "internal/USBInterrupt.h"

#include "internal/Device.h"
#include "internal/Endpoint.h"
#include "internal/DeviceStandardReq.h"
#include "internal/EndpointStream.h"

#include "internal/CDC.h"
