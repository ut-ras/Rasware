#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>

//tLineSensor *ls;

void initLineSensor(void) {
    //tI2C *bus = InitializeI2C(PIN_B3, PIN_B2);
    //ls = InitializeLineSensor(bus, 0);
		InitializeLineSensor();
}

void lineSensorDemo(void) {
    Printf("Press:\nany key-read line sensor\n");
    Printf("any key after read begins-quit\n");
  
    while(!KeyWasPressed()) {
				int i;
        char *line;
				
				ReadLineSensor(line);
        /*
				float line[8];
    
        LineSensorReadArray(ls, line);
        Printf("Line Sensor: [");
				*/
        for (i=0; i < 8; i++) {
            Printf("%x ", line[i]);
        }
    
        Printf("\r");
      }
  
      Printf("\n"); 
}
