ARDPATH  = /Applications/Arduino.app/Contents/Java
BINPATH  = $(ARDPATH)/hardware/tools/avr/bin
CONFFILE = $(ARDPATH)/hardware/tools/avr/etc/avrdude.conf
COM      = /dev/cu.usbmodem1411
PROTOCOL = avr109
SPEED    = 57600
F_CPU    = 16000000L

ARDUBOY_MODEL = ARDUBOY_10
MCU_NAME      = atmega32u4
DEV_NAME      = "Arduino Leonardo"

#=============================================================================

PREFIX  = $(BINPATH)/avr-
CPP	    = $(PREFIX)g++
CC      = $(PREFIX)gcc
AR      = $(PREFIX)ar
LD      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
NM      = $(PREFIX)nm

SRCDIR  = src
LIBDIR  = $(SRCDIR)/lib
INCDIR  =

#=============================================================================

CFILES = \
	$(wildcard $(SRCDIR)/*.c) \
	$(wildcard $(SRCDIR)/**/*.c) \

OBJDIR     = obj
OUTDIR     = bin
NAME       = arduboy
MAPFILE    = $(OUTDIR)/$(NAME).map
NMFILE     = $(OUTDIR)/$(NAME).nm
TARGET_ELF = $(OUTDIR)/$(NAME).elf
TARGET_HEX = $(OUTDIR)/$(NAME).hex

#=============================================================================

DFLAGS = \
    -D F_CPU=$(F_CPU) \
    -D ARDUINO=10606 \
    -D ARDUINO_AVR_LEONARDO \
    -D ARDUINO_ARCH_AVR \
    -D USB_VID=0x2341 \
    -D USB_PID=0x8036 \
    -D USB_MANUFACTURER="Unknown" \
    -D $(ARDUBOY_MODEL)

CFLAGS = \
    -c \
    -g \
    -Os \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffunction-sections \
    -fdata-sections \
    -MMD \
    -mmcu=$(MCU_NAME) \
    $(DFLAGS) \
    -DUSB_PRODUCT=$(DEV_NAME) \
    $(foreach incdir,$(INCDIR),-I$(incdir)) \
    -x c

LDFLAGS = \
    -Wall \
    -Wextra \
    -Os \
    -Wl,--gc-sections \
    -Wl,-Map=$(MAPFILE) \
    -mmcu=$(MCU_NAME)

#=============================================================================
# LUFA Stuff

VID                  = 0x2341UL
PID                  = 0x0036UL
BOARD                = USER
F_USB                = $(F_CPU)
FLASH_SIZE_KB        = 32
BOOT_SECTION_SIZE_KB = 4
BOOT_START           = 0x$(shell echo "obase=16; ($(FLASH_SIZE_KB) - $(BOOT_SECTION_SIZE_KB)) * 1024" | bc)
LUFA_PATH            = $(SRCDIR)/LUFA

# LUFA library compile-time options and predefined tokens
DFLAGS  += \
    -D F_USB=$(F_USB) \
    -D BOARD=BOARD_$(BOARD) \
    -D DEVICE_VID=$(VID) \
	-D DEVICE_PID=$(PID) \
    -D USB_DEVICE_ONLY \
    -D DEVICE_STATE_AS_GPIOR=0 \
    -D ORDERED_EP_CONFIG \
    -D FIXED_CONTROL_ENDPOINT_SIZE=8 \
    -D FIXED_NUM_CONFIGURATIONS=1 \
    -D USE_RAM_DESCRIPTORS \
    -D USE_STATIC_OPTIONS="(USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)" \
    -D NO_INTERNAL_SERIAL \
    -D NO_DEVICE_SELF_POWER \
    -D NO_DEVICE_REMOTE_WAKEUP \
    -D NO_SOF_EVENTS \
    -D NO_LOCK_BYTE_WRITE_SUPPORT

# Define module source file lists
CFILES += \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/Device_AVR8.c         \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/Endpoint_AVR8.c       \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/Host_AVR8.c           \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/Pipe_AVR8.c           \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/USBController_AVR8.c  \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/USBInterrupt_AVR8.c   \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/EndpointStream_AVR8.c \
    $(LUFA_PATH)/Drivers/USB/Core/AVR8/PipeStream_AVR8.c     \
    $(LUFA_PATH)/Drivers/USB/Core/ConfigDescriptor.c         \
    $(LUFA_PATH)/Drivers/USB/Core/DeviceStandardReq.c        \
    $(LUFA_PATH)/Drivers/USB/Core/Events.c                   \
    $(LUFA_PATH)/Drivers/USB/Core/USBTask.c

INCDIR += $(SRCDIR)

#=============================================================================

VPATH  = $(dir $(CFILES))
OFILES = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(CFILES))))
DFILES = $(addprefix $(OBJDIR)/, $(addsuffix .d, $(notdir $(CFILES))))

#=============================================================================

.PHONY: all usb clean

all:
	@make $(TARGET_HEX)

size: all
	$(BINPATH)/avr-size $(TARGET_ELF) -C --mcu=$(MCU_NAME)

usb:
	@python tools/reset.py $(COM)
	@$(BINPATH)/avrdude -C$(CONFFILE) -p$(MCU_NAME) -c$(PROTOCOL) -P$(COM) -b$(SPEED) -D -Uflash:w:$(TARGET_HEX):i

clean:
	@rm -rf $(OBJDIR) $(OUTDIR) $(MAPFILE) $(NMFILE) $(TARGET_ELF) $(TARGET_HEX)

#=============================================================================

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET_ELF): $(OBJDIR) $(OUTDIR) $(OFILES) Makefile
	$(AR) rcs $(OBJDIR)/core.a $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $(OBJDIR)/core.a
	$(NM) -n $@ > $(NMFILE)

$(OBJDIR):
	mkdir $@

$(OUTDIR):
	mkdir $@

#=============================================================================

.SUFFIXES: .c .o .a .d

$(OBJDIR)/%.c.o: %.c
	$(CC) $(CFLAGS) $< -o $@

-include $(DFILES)
