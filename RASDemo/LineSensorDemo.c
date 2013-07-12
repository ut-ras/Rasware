#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART	 
#include <driverlib/uart.h>		// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/linesensor.h>

#include "RASDemo.h"

void initLineSensor(void) {
	//InitializeLineSensor();
}

void lineSensorDemo(void) {
	UARTprintf("Press:\nany key-read line sensor\n");
	UARTprintf("any key after read begins-quit\n");
	
	UARTprintf("\nNot working.\n");
	/*while(!keyWasPressed()) {
		unsigned char lineSensorByte = ReadLineSensor();
	 	int lineSensorArray[8];
		int i;
		for(i = 0; i < 8; i++) {
			lineSensorArray[i] = lineSensorByte & 0x01;
			lineSensorByte = lineSensorByte >> 1;
		}
		
 		//black is 1, white is 0
		for(i = 0; i < 8; i++) 
			UARTprintf("%u",lineSensorArray[i]);
		UARTprintf("\r");  
	}*/
	UARTprintf("\n");
}
