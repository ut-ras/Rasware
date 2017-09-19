# Makefile for RASWare 2013
TARGET = RASTemplate.out

# Library Locations
RASLIB = ../RASLib
STELLARIS = /usr/local/lib/StellarisWare
CORTEXM4 = /usr/local/lib/CortexM4Libs
ARMCLIB = $(dir $(shell which $(CC)))..
GDBSCRIPT = $(RASLIB)/gdb-script

# Board device path
DETECT_SCRIPT = $(RASLIB)/detect-board

# Command Definitions
PREFIX = arm-none-eabi-

CC := $(PREFIX)gcc
LD := $(PREFIX)ld
AR := $(PREFIX)ar
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size
GDB := $(PREFIX)gdb

OPENOCDCFG := $(shell find /usr -path */scripts/board/*tm4c123* 2>/dev/null)
OPENOCD = openocd -f $(OPENOCDCFG)
SCREEN := screen

# File Definitions
SRC := $(wildcard src/*.c *.c)
INC = inc $(RASLIB)/.. $(STELLARIS)/.. $(STELLARIS)
LIB = ras driver-cm4f m c gcc
LDS = lm4f.ld

OBJ := $(SRC:.c=.o)
ASM := $(SRC:.c=.s)

# Flag Definitions
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m4

ifneq ($(filter MINGW%,$(shell uname)),)
lib-search-dirs = $(shell ${CC} ${CFLAGS} -print-search-dirs | grep libraries | sed -e "s/libraries:\ =/-L'/" -e "s/ /\\\ /" -e "s/;/' -L'/g" -e "s/$$/'/")
else
lib-search-dirs = $(shell ${CC} ${CFLAGS} -print-search-dirs | grep libraries | sed -e "s/libraries:\ =/-L'/" -e "s/ /\\\ /" -e "s/:/' -L'/g" -e "s/$$/'/")
endif
LDDIR += $(RASLIB)/output
LDDIR += $(STELLARIS)/driverlib/gcc-cm4f
LDDIR += $(CORTEXM4)

CFLAGS += -g
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -mfloat-abi=softfp
CFLAGS += -O0
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -MD
CFLAGS += -std=c99
CFLAGS += -Dgcc
CFLAGS += -DPART_LM4F120H5QR
CFLAGS += -DTARGET_IS_BLIZZARD_RA1

LDFLAGS += --entry ResetHandler
LDFLAGS += --gc-sections

CFLAGS += $(addprefix -I, $(INC))
LDFLAGS += $(addprefix -L, $(LDDIR))
LDFLAGS += ${lib-search-dirs}
LDFLAGS += $(addprefix -l, $(LIB))

# Rules
all: $(TARGET)

-include $(OBJECTS:.o=.d)

asm: $(ASM)

size: $(TARGET)
	$(SIZE) $<

flash: $(TARGET)
	$(OPENOCD) -c "init"                              \
	           -c "halt"                              \
	           -c "flash write_image erase $(TARGET)" \
	           -c "verify_image $(TARGET)"            \
	           -c "halt"                              \
	           -c "shutdown"

debug: $(TARGET)
	$(GDB) $< -x $(RASLIB)/gdb-script

uart:
	$(SCREEN) $(shell $(DETECT_SCRIPT)) 115200

run: flash
	$(SCREEN) $(shell $(DETECT_SCRIPT)) 115200

clean:
	-rm $(TARGET)
	-rm $(TARGET:.axf=.out)
	-rm $(ASM)
	-rm $(OBJ:.o=.d)


%.a: $(OBJ)
	$(AR) rcs $@ $^

%.axf: %.out
	$(OBJCOPY) -O binary $< $@

%.out: $(OBJ)
	$(LD) -T $(LDS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.s: %.c
	$(CC) -S $(CFLAGS) -o $@ $<
