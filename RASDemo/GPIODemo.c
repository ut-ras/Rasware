#include "RASDemo.h"
#include <RASLib/inc/gpio.h>

#include <RASLib/inc/common.h>


void gpioDemo(void) {
    Printf("Press:\nany key-read port A values\n");
    Printf("any key after read begins-quit\n");

    while(!KeyWasPressed()) {
        tPin i;
        
        Printf("port A: ");
        for (i = PIN_A0; i <= PIN_A7; i++) {
            Printf(GetPin(i) ? "1" : "0");
        }
        
        Printf("\n");
      }
    
      Printf("\n");
}
