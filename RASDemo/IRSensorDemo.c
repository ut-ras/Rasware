#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/uart.h>		// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <driverlib/adc.h>

#include "RASDemo.h"

void initIRSensor(void) {
	/*SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE,0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC_BASE, 0);*/
	UARTprintf("init IRSensor not working.\n");
}

long getADCValue(void) {
	unsigned long ADCValue = 0;
	ADCProcessorTrigger(ADC_BASE, 0 ); 
	while(!ADCIntStatus(ADC_BASE, 0, false)); 
	ADCSequenceDataGet(ADC_BASE, 0, &ADCValue);
	return ADCValue;
}

void IRSensorDemo(void) {
	/* 2012 IRSensor Demo
	UARTprintf("Press:\nany key-read IR sensor\n");
	UARTprintf("any key after read begins-quit\n");
	
	while(!keyWasPressed()) {
		unsigned long ADCValue = getADCValue();
	 	UARTprintf("IR value: %d\r",ADCValue);
	}	
	UARTprintf("\n");*/
	
	UARTprintf("IRSensor Demo not working.\n");
}
