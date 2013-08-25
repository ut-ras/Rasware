#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <driverlib/adc.h>

#include <RASLib/inc/adc.h>
#include <RASLib/inc/uart.h>

#include "RASDemo.h"

tADC *adc[4];

void initIRSensor(void) {
	/*SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE,0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC_BASE, 0);*/
	adc[0] = InitializeADC(PIN_E2);
    adc[1] = InitializeADC(PIN_E3);
    adc[2] = InitializeADC(PIN_E4);
    adc[3] = InitializeADC(PIN_E5);
	//printf("init IRSensor not working.\n");
}

/*long getADCValue(void) {
	unsigned long ADCValue = 0;
	ADCProcessorTrigger(ADC_BASE, 0 ); 
	while(!ADCIntStatus(ADC_BASE, 0, false)); 
	ADCSequenceDataGet(ADC_BASE, 0, &ADCValue);
	return ADCValue;
}*/

void IRSensorDemo(void) {
	// 2012 IRSensor Demo
	printf("press any key to quit\n");
    
    ADCReadContinuouslyUS(adc[0], 0);
    ADCReadContinuouslyUS(adc[1], 0);
    ADCReadContinuouslyUS(adc[2], 0);
    ADCReadContinuouslyUS(adc[3], 0);
	
	while(!keyWasPressed()) {
		float ADCValue = ADCRead(adc[0]);
	 	printf("IR value: %d\t", (int)(1000 * ADCValue));
        ADCValue = ADCRead(adc[1]);
	 	printf(" %d\t", (int)(1000 * ADCValue));
        ADCValue = ADCRead(adc[2]);
	 	printf(" %d\t", (int)(1000 * ADCValue));
        ADCValue = ADCRead(adc[3]);
	 	printf(" %d\r", (int)(1000 * ADCValue));
	}	
	printf("\n");
	
	//printf("IRSensor Demo not working.\n");
}
