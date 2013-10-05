#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>

tLineSensor *ls;

void initLineSensor(void) {
    tI2C *bus = InitializeI2C(PIN_B3, PIN_B2);
    ls = InitializeLineSensor(bus, 0);
}

void lineSensorDemo(void) {
    Printf("Press:\nany key-read line sensor\n");
    Printf("any key after read begins-quit\n");
  
    while(!KeyWasPressed()) {
        int i;
        float line[8];
    
        LineSensorReadArray(ls, line);
        Printf("Line Sensor: [");
    
        for (i=0; i < 8; i++) {
            Printf("%01.4f ", line[i]);
        }
    
        Printf("\b]\r");
      }
  
      Printf("\n"); 
}
