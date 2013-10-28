#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include "GPIOLineSensor.h"

void initLineSensor(void) {
		// Line sensor plugs into these pins:
		// PB5,PD0,PD1,PD2,PD3,PE0,PC6,PC7
		// (the top 8 on the 3.3V rail on the 2013 booster pack)
		InitializeLineSensor();
}

void lineSensorDemo(void) {
    Printf("Press:\nany key-read line sensor\n");
    Printf("any key after read begins-quit\n");
  
    while(!KeyWasPressed()) {
				int i;
        char line[8];
				
				ReadLineSensor(line);
        for (i=0; i < 8; i++) {
            Printf("%x ", line[i]);
        }
        Printf("\r");
      }
      Printf("\n"); 
}
