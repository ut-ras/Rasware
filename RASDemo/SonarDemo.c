#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/sonar.h>

tSonar *demoSonar;
tBoolean initialized = false;

void initSonar(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    demoSonar = InitializeSonar(PIN_A3, PIN_A4);
}

void sonarDemo(void) {
    Printf("Press any key to quit\n");
   
    // set the sonar to be continuously updated in the background 
    SonarReadContinuously(demoSonar, 0);
  
    // loop as long as the user doesn't press a key 
    while (!KeyWasPressed()) {
        Printf("Sonar value: %1.3f\r", SonarRead(demoSonar));
    }
  
    Printf("\n");
}
