#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART	 
#include <driverlib/uart.h>		// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/linesensorI2C.h>

#include "RASDemo.h"

void InitializeLineSensor(void) {
	initLineSensor();
}

void lineSensorDemo(void) {
	// 2012 Line Sensor Demo
	char lineSensorArray[8];
	char filteredArray[8]; 
	int i; 
	
	UARTprintf("Press:\nany key-read line sensor\n");
	UARTprintf("any key after read begins-quit\n");
	while(!keyWasPressed()) {
		readLineSensor(lineSensorArray);
		UARTprintf("Line Sensor: ");
		for(i = 0; i < 8; i++)
    {	
			if(lineSensorArray[i]>100) {
				filteredArray[i] = 1; 
			}
			else filteredArray[i] = 0; 
			UARTprintf("%u      ",lineSensorArray[i]);
	  }
		UARTprintf("\r"); 
		
	}
	UARTprintf("\n"); 
}
