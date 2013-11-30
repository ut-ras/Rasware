#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/sonar.h>

tSonar *demoSonar;

void initSonar(void) {
    demoSonar = InitializeSonar(PIN_A3, PIN_A4);
}

void sonarDemo(void) {
    Printf("Press any key to quit\n");
    
    SonarReadContinuously(demoSonar, 0);
  
    while (!KeyWasPressed()) {
        Printf("Sonar value: %1.3f\r", SonarRead(demoSonar));
    }
  
    Printf("\n");
}
