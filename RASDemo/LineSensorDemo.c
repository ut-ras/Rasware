#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/uart.h>

#include "RASDemo.h"

void lineSensorDemo(void) {
	unsigned char lineSensorArray[8];
//	char filteredArray[8]; 
	int i; 

	Printf("Press:\nany key-read line sensor\n");
	Printf("any key after read begins-quit\n");
	while(!KeyWasPressed()) {
		ReadLineSensorArray(lineSensorArray);
		Printf("Line Sensor: ");
		for(i = 0; i < 8; i++)
    {	
			/*
			if(lineSensorArray[i]>100) {
				filteredArray[i] = 1; 
			}
			else filteredArray[i] = 0; 
			*/
			Printf("%u      ",lineSensorArray[i]);
	  }
		Printf("\r"); 

	}
	Printf("\n"); 
}
