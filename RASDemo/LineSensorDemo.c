#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/uart.h>

#include "RASDemo.h"

tLineSensor *ls;

void initLineSensor(void) {
	ls = InitializeLineSensor(PIN_B3, PIN_B2, 0);
}

void lineSensorDemo(void) {
	// 2012 Line Sensor Demo
	Printf("Press:\nany key-read line sensor\n");
	Printf("any key after read begins-quit\n");
	while(!KeyWasPressed()) {
        int i;
        float line[8];
        
        LineSensorReadArray(ls, line);
		Printf("Line Sensor: [");
        
        for (i=0; i < 8; i++) {
            Printf("%f ", line[i]);
        }
        
        Printf("\b]\r");
	}
	Printf("\n"); 
}
