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

    // initialize an I2C bus with a couple of pins
    tI2C *bus = InitializeI2C(PIN_B3, PIN_B2);
    
    // intialize a line sensor to be on the I2C bus at address 0
    ls = InitializeI2CLineSensor(bus, 0);
}

void i2cLineSensorDemo(void) {
    Printf("Press any key to quit\n");
 
    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        int i;
        float line[8];
    
        // put the values of the line sensor into the 'line' array 
        LineSensorReadArray(ls, line);
        Printf("Line Sensor: [");
    
        for (i = 0; i < 8; i++) {
            Printf("%01.4f ", line[i]);
        }
    
        Printf("\b]\r");
    }
  
    Printf("\n"); 
}

