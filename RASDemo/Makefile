#
# Defines the directory suffix that this project uses.
#

#
# Set the processor variant.
#
VARIANT=cm3

SUFFIX=-${VARIANT}
#
# The base directory for StellarisWare.
#
ROOT=..

#
# Include the common make definitions.
#
include /opt/arm-2011.03/share/makedefs

#
# Where to find header files that do not live in the source directory.
#

IPATH=/opt/arm-2011.03/include/

#
# The default rule, which causes the driver library to be built.
#
all: ${COMPILER}
all: ${COMPILER}/main.axf

#
# The rule to clean out all the build products.
#
clean:
	@killall openocd || true
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to start the openOCD server
#
openocd: ${COMPILER}/main.axf
	@openocd -f /usr/share/openocd/scripts/board/ek-lm3s811.cfg 1>/dev/null 2>/dev/null &

#
# The rule to start gdb on the device
#
gdb:${COMPILER} openocd ${COMPILER}/main.axf
	@arm-none-eabi-gdb ${COMPILER}/main.axf -ex "target remote localhost:3333"

#
# The rule to flash the device with an image
#
flash: ${COMPILER} openocd ${COMPILER}/main.axf
	@echo flashing board
	@./telnet.py "halt" "flash write_image erase ${COMPILER}/main.axf"

#
# uart rule; just run screen
#
uart: 
	@./telnet.py "reset run"
	@screen /dev/lm3s 115200

flash+gdb: flash gdb

flash+uart: flash uart
#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the driver library.
#
${COMPILER}/main.axf: ${COMPILER}/Main.o
${COMPILER}/main.axf: ${COMPILER}/EncoderDemo.o
${COMPILER}/main.axf: ${COMPILER}/IRSensorDemo.o
${COMPILER}/main.axf: ${COMPILER}/LineSensorDemo.o
${COMPILER}/main.axf: ${COMPILER}/ServoDemo.o
${COMPILER}/main.axf: ${COMPILER}/MotorDemo.o
${COMPILER}/main.axf: ${COMPILER}/UARTDemo.o
${COMPILER}/main.axf: ${COMPILER}/startup.o

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}-cm3/*.d} __dummy__
endif
