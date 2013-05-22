//*****************************************************************************
//
// adc.c - analog to digital converter drivers
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE AUTHORS OF THIS FILE
// SHALL NOT, UNDER ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of RASLib Rev0 of the RASWare2013 package.
//
// Written by: 
// The student branch of the 
// IEEE - Robotics and Automation Society 
// at the University of Texas at Austin
//
// Website: ras.ece.utexas.edu
// Contact: rasware@ras.ece.utexas.edu
//
//*****************************************************************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "adc.h"

/***************** GLOBAL VARIABLES *****************/
unsigned long g_rgADCValues[2];


// Initializes two ADC pins, PE2 = ADC1 and PE3 = ADC0
// Free running, values to be read from mailbox g_rgADCValues
// 128khz Sampling rate with 64 sample hardware averaging = 2ks/s
// Rates can be changed by the #defines in the header
// Uses GPIOPin E2 and E3, ADC0 Sequencer 1, and Timer5B
void InitializeADC(void)
{
    // Give intial values to the globals
    g_rgADCValues[0] = 0;
    g_rgADCValues[1] = 0;
    
    // Enable clocking for Timer5, ADC0, and GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
    // Set PE2 and PE3 to be ADC pins
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    
    // Configure Timer5B to be periodic, maintaining the configuration for Timer5A
    TimerConfigure(TIMER5_BASE, TIMER5_CFG_R | TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
	
    // Enable the Timer5B interrupt
    TimerIntEnable(TIMER5_BASE, TIMER_TIMB_TIMEOUT );
    
    // Enable sample sequence 1 with a processor signal trigger.  Sequence 1 will 
    // do up to four samples when the processor sends a signal to start the conversion.
    // Each ADC module has 4 programmable sequences, sequence 0 to sequence 3.
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    
    // Configure step 0 and 1 on sequence 1.  Sample channel 0 (ADC_CTL_CH0),
    // then sample channel 1 (ADC_CTL_CH1) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 1 (ADC_CTL_END).
    /// Sequence 3 has only one step, sequence 1 and 2 have 4 steps, and sequence 0 has 8 programmable steps.   
    // For more information on the ADC sequences and steps, refer to the datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0,  ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1,  ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
    
    // Enable hardware oversampling (higher precision results)
    ADCHardwareOversampleConfigure( ADC0_BASE , HW_OVERSAMPLING_FACTOR );
    
    // Load Timer5B with a frequency of ADC_SAMPLE_RATE * HW_OVERSAMPLING_FACTOR
    TimerLoadSet(TIMER5_BASE, TIMER_B, SysCtlClockGet() / ADC_SAMPLE_RATE / HW_OVERSAMPLING_FACTOR);
    
    // Enable ADC0:Sequence 1
    ADCSequenceEnable(ADC_BASE, 1);
    
    // Enable ADC0:Sequence 1 interrupt
    ADCIntEnable(ADC0_BASE, 1);
    
    // Enable Timer5B
    TimerEnable(TIMER5_BASE, TIMER_B);
    
    // Enable interrupts
    IntEnable(INT_ADC0SS1);
    IntEnable(INT_TIMER5B);
}


// Returns the latest value read from the specified ADC pin
// \param index is the ADC pin to read ( 0 = PE3, 1 = PE2)
// \return latest value read from the specified ADC pin
unsigned long GetADC(int index) { return g_rgADCValues[index]; }

// Interrupt handler for ADC0:Sequencer 1
void ADC0SS1Handler(void){
	// Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC_BASE, 1, g_rgADCValues);
    
    // Clear the interrupt
    ADCIntClear(ADC_BASE, 1);
}

// Interrupt handler for Timer5B
void ADCTriggerHandler(void){
    // Trigger ADC0:Sequence1
    ADCProcessorTrigger(ADC0_BASE, 1);
    
    // Clear the interrupt
    TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
}
