#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>

tLineSensor *gls;
tBoolean initialized = false;

void initGPIOLineSensor(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    // use 8 I/O pins to initialize a GPIO line sensor
    gls = InitializeGPIOLineSensor(
        PIN_B5, 
        PIN_D0, 
        PIN_D1, 
        PIN_D2, 
        PIN_D3, 
        PIN_E0, 
        PIN_C6, 
        PIN_C7
        );
}

void gpioLineSensorDemo(void) {
    Printf("Press any key to quit\n");
  
    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        int i;
        float line[8];
   
        // put the values of the line sensor into the 'line' array 
        LineSensorReadArray(gls, line);
        Printf("Line Sensor: [");
    
        for (i = 0; i < 8; i++) {
            Printf("%01.4f ", line[i]);
        }
    
        Printf("\b]\r");
    }
  
    Printf("\n"); 
}

