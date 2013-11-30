#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/adc.h>

tADC *adc[4];

void initIRSensor(void) {
    adc[0] = InitializeADC(PIN_D0);
    adc[1] = InitializeADC(PIN_D1);
    adc[2] = InitializeADC(PIN_D2);
    adc[3] = InitializeADC(PIN_D3);
}

void IRSensorDemo(void) {
    Printf("Press any key to quit\n");
        
    while (!KeyWasPressed()) {
        Printf(
            "IR values:  %1.3f  %1.3f  %1.3f  %1.3f\r",
            ADCRead(adc[0]),
            ADCRead(adc[1]),
            ADCRead(adc[2]),
            ADCRead(adc[3])
            );
    }
  
    Printf("\n");
}
