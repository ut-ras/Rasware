#include "RASDemo.h"

#include <RASLib/inc/gpio.h>
#include <RASLib/inc/common.h>

void gpioDemo(void) {
    Printf("Press any key to quit\n");

    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        tPin i;
        
        // get the values of each pin in port A and display them
        Printf("port A: ");
        for (i = PIN_A0; i <= PIN_A7; i++) {
            Printf(GetPin(i) ? "1" : "0");
        }
        
        Printf("\n");
    }
    
    Printf("\n");
}

