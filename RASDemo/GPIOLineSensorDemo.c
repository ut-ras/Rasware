#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>

tLineSensor *gls;

void initGPIOLineSensor(void) {
    gls = InitializeGPIOLineSensor0();
}

void gpioLineSensorDemo(void) {
    Printf("Press:\nany key-read line sensor\n");
    Printf("any key after read begins-quit\n");
  
    while(!KeyWasPressed()) {
        int i;
        float line[8];
    
        LineSensorReadArray(gls, line);
        Printf("Line Sensor: [");
    
        for (i=0; i < 8; i++) {
            Printf("%01.4f ", line[i]);
        }
    
        Printf("\b]\r");
      }
  
      Printf("\n"); 
}
