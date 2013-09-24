#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/adc.h>

tADC *adc[4];

void initIRSensor(void) {
  adc[0] = InitializeADC(PIN_E4);
  adc[1] = InitializeADC(PIN_E5);
  adc[2] = InitializeADC(PIN_E1);
  adc[3] = InitializeADC(PIN_B4);
}

void IRSensorDemo(void) {
  Printf("Press any key to quit\n");
    
  ADCReadContinuouslyUS(adc[0], 0);
  ADCReadContinuouslyUS(adc[1], 0);
  ADCReadContinuouslyUS(adc[2], 0);
  ADCReadContinuouslyUS(adc[3], 0);
  
  while(!KeyWasPressed()) {
    float ADCValue = ADCRead(adc[0]);
    Printf("IR values: %d\t", (int)(1000 * ADCValue));
      ADCValue = ADCRead(adc[1]);
    Printf(" %d\t", (int)(1000 * ADCValue));
      ADCValue = ADCRead(adc[2]);
    Printf(" %d\t", (int)(1000 * ADCValue));
      ADCValue = ADCRead(adc[3]);
    Printf(" %d\r", (int)(1000 * ADCValue));
  }
  
  Printf("\n");
}
