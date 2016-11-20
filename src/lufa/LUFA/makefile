LUFA_ROOT_PATH = $(LUFA_PATH)/LUFA
ARCH = AVR8

# Define module source file lists
LUFA_SRC_USB          = $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Device_$(ARCH).c         \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Endpoint_$(ARCH).c       \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Host_$(ARCH).c           \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Pipe_$(ARCH).c           \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/USBController_$(ARCH).c  \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/USBInterrupt_$(ARCH).c   \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/EndpointStream_$(ARCH).c \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/PipeStream_$(ARCH).c     \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/ConfigDescriptor.c               \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/DeviceStandardReq.c              \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/Events.c                         \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/HostStandardReq.c                \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Core/USBTask.c                        \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Common/HIDParser.c
LUFA_SRC_USBCLASS     = $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/Audio.c                  \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/CDC.c                    \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/HID.c                    \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/MassStorage.c            \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/MIDI.c                   \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/RNDIS.c                  \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/Audio.c                    \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/CDC.c                      \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/HID.c                      \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/MassStorage.c              \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/MIDI.c                     \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/Printer.c                  \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/RNDIS.c                    \
                        $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/StillImage.c
LUFA_SRC_TEMPERATURE  = $(LUFA_ROOT_PATH)/Drivers/Board/Temperature.c
LUFA_SRC_SERIAL       = $(LUFA_ROOT_PATH)/Drivers/Peripheral/$(ARCH)/Serial_$(ARCH).c
LUFA_SRC_TWI          = $(LUFA_ROOT_PATH)/Drivers/Peripheral/$(ARCH)/TWI_$(ARCH).c
LUFA_SRC_SCHEDULER    = $(LUFA_ROOT_PATH)/Scheduler/Scheduler.c
