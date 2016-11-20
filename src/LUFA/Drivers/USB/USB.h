/*
 *  Master include file for the library USB functionality.
 *
 *  This file should be included in all user projects making use of the USB portions of the library, instead of
 *  the individual USB driver submodule headers.
 */

#pragma once

#define __INCLUDE_FROM_USB_DRIVER

#include "../../Common/Common.h"
#include "Core/USBMode.h" // done

#include "Core/USBTask.h"
#include "Core/Events.h"
#include "Core/StdDescriptors.h"
#include "Core/ConfigDescriptor.h"
#include "Core/USBController.h"
#include "Core/USBInterrupt.h"

#include "Core/Device.h"
#include "Core/Endpoint.h"
#include "Core/DeviceStandardReq.h"
#include "Core/EndpointStream.h"

#include "Class/CDC.h"
#include "Class/HID.h"
#include "Class/MassStorage.h"
#include "Class/Printer.h"
#include "Class/RNDIS.h"
#include "Class/StillImage.h"
