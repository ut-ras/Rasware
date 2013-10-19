# Makefile for RASWare 2013
TARGET = output/libras.a

# Library Locations
STELLARIS = ../../StellarisWare
RASLIB = ../RASLib
ARMCLIB = $(dir $(shell which $(CC)))..

# File Definitions
SRC := $(wildcard src/*.c)
INC = inc $(RASLIB)/.. $(STELLARIS)/.. $(STELLARIS)
LIB = driver-cm4f m c gcc
LDS = lm4f.ld

OBJ := $(SRC:.c=.o)
ASM := $(SRC:.c=.s)

# Command Definitions
PREFIX = arm-none-eabi

CC := $(PREFIX)-gcc
LD := $(PREFIX)-ld
OBJCOPY := $(PREFIX)-objcopy
SIZE := $(PREFIX)-size

FLASH := lm4flash
SCREEN := screen

# Flag Definitions
CFLAGS += -g
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m4
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
LDFLAGS += -L$(RASLIB)/output
LDFLAGS += -L$(STELLARIS)/driverlib/gcc-cm4f
LDFLAGS += -L$(ARMCLIB)/$(PREFIX)/lib
LDFLAGS += -L$(wildcard $(ARMCLIB)/lib/gcc/$(PREFIX)/*)

CFLAGS += $(addprefix -I, $(INC))
LDFLAGS += $(addprefix -l, $(LIB))

-include $(OBJECTS:.o=.d)

# Rules
all: $(TARGET)

asm: $(ASM)

size: $(TARGET:.axf=.out)
	$(SIZE) $<

flash: $(TARGET)
	$(FLASH) $<

uart:
	$(SCREEN) /dev/lm4f 115200

clean:
	-rm $(TARGET)
	-rm $(TARGET:.axf=.out)
	-rm $(ASM)
	-rm $(OBJ:.o=.d)


%.a: $(OBJ)
	$(AR) rc $@ $^

%.axf: %.out
	$(OBJCOPY) -O binary $< $@

%.out: $(OBJ)
	$(LD) -T $(LDS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.s: %.c
	$(CC) -S $(CFLAGS) -o $@ $<
