#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/uart.h>

#include "RASDemo.h"

void initLineSensor(void) {
	//InitializeLineSensor();
	Printf("init Line not working.\n");
}

void lineSensorDemo(void) {
	/* 2012 Line Sensor Demo
	Printf("Press:\nany key-read line sensor\n");
	Printf("any key after read begins-quit\n");
	while(!keyWasPressed()) {
		unsigned char lineSensorByte = ReadLineSensor();
	 	int lineSensorArray[8];
		int i;
		for(i = 0; i < 8; i++) {
			lineSensorArray[i] = lineSensorByte & 0x01;
			lineSensorByte = lineSensorByte >> 1;
		}
		
 		//black is 1, white is 0
		for(i = 0; i < 8; i++) 
			Printf("%u",lineSensorArray[i]);
		Printf("\r");  
	}
	Printf("\n");*/
	
	Printf("Line Sensor Demo not working.\n");
}
