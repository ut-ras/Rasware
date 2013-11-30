#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>

tLineSensor *ls;
tBoolean initialized = false;

void initI2CLineSensor(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    tI2C *bus = InitializeI2C(PIN_B3, PIN_B2);
    ls = InitializeI2CLineSensor(bus, 0);
}

void i2cLineSensorDemo(void) {
    Printf("Press any key to quit\n");
  
    while (!KeyWasPressed()) {
        int i;
        float line[8];
    
        LineSensorReadArray(ls, line);
        Printf("Line Sensor: [");
    
        for (i = 0; i < 8; i++) {
            Printf("%01.4f ", line[i]);
        }
    
        Printf("\b]\r");
    }
  
    Printf("\n"); 
}

