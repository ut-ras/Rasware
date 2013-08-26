#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART	 
#include <driverlib/uart.h>		// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/linesensor.h>

#include "RASDemo.h"

tLineSensor *ls;

void initLineSensor(void) {
	ls = InitializeLineSensor(PIN_B3, PIN_B2);
}

void lineSensorDemo(void) {
	// 2012 Line Sensor Demo
	UARTprintf("Press:\nany key-read line sensor\n");
	UARTprintf("any key after read begins-quit\n");
	while(!keyWasPressed()) {
		UARTprintf("Line Sensor: %x\n", LineSensorRead(ls, 0.5f));
	}
	UARTprintf("\n"); 
}
