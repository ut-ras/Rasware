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
unsigned long g_rgADCValues[ADC_BUFFER_SIZE];


//****** Definition of ADC Pin-map for the TM4C1233H6PM / LM4F120H5QR ******/
//****** ADC#  ***** Pin Assignement
//****** ----- ***** ------------
//****** AIN0  *****     PE3 
//****** AIN1  *****     PE2 
//****** AIN2  *****     PE1
//****** AIN3  *****     PE0
//****** AIN4  *****     PD3
//****** AIN5  *****     PD2
//****** AIN6  *****     PD1
//****** AIN7  *****     PD0
//****** AIN8  *****     PE5
//****** AIN9  *****     PE4
//****** AIN10 *****     PB4
//****** AIN11 *****     PB5

static const unsigned long ADCPortBase[] = {
    GPIO_PORTE_BASE, 
    GPIO_PORTE_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTB_BASE,
    GPIO_PORTB_BASE
};
static const unsigned char ADCGPIOPin[] = {
    GPIO_PIN_3,
    GPIO_PIN_2,
    GPIO_PIN_1,
    GPIO_PIN_0,
    GPIO_PIN_3,
    GPIO_PIN_2,
    GPIO_PIN_1,
    GPIO_PIN_0,
    GPIO_PIN_5,
    GPIO_PIN_4,
    GPIO_PIN_4,
    GPIO_PIN_5
};

// Initializes primary ADC controller
// Free running, values to be read from mailbox g_rgADCValues
// 128khz Sampling rate with 64 sample hardware averaging = 2ks/s
// Rates can be changed by the #defines in the header
// Uses ADC0 and 1, Sequencers 0 through 2 on each, and Timer5B
static void InitializeADCMain(void){
    int i;
    // Give intial values to the globals
    for( i = 0; i < ADC_BUFFER_SIZE; i++ ){
        g_rgADCValues[i]=0;
    }
    
    // Enable clocking for Timer5, ADC0, ADC1, PortB, PortD, and PortE 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    
    // Configure Timer5B to be periodic, maintaining the configuration for Timer5A
    TimerConfigure(TIMER5_BASE, TIMER5_CFG_R | TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
	
    // Enable the Timer5B interrupt
    IntEnable(INT_TIMER5B);
    TimerIntEnable(TIMER5_BASE, TIMER_TIMB_TIMEOUT );
    
    // Enable sample sequence 1 with a processor signal trigger.  Sequence 1 will 
    // do up to four samples when the processor sends a signal to start the conversion.
    // Each ADC module has 4 programmable sequences, sequence 0 to sequence 3.
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 2);
    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceConfigure(ADC1_BASE, 2, ADC_TRIGGER_PROCESSOR, 2);
    
    // Configure step 0 and 1 on sequences .  Sample channel 0 (ADC_CTL_CH0),
    // then sample channel 1 (ADC_CTL_CH1) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 1 (ADC_CTL_END).
    /// Sequence 3 has only one step, sequence 1 and 2 have 4 steps, and sequence 0 has 8 programmable steps.   
    // For more information on the ADC sequences and steps, refer to the datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,  ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1,  ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0,  ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1,  ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0,  ADC_CTL_CH4);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1,  ADC_CTL_CH5 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0,  ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 1,  ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 0,  ADC_CTL_CH8);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 1,  ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC1_BASE, 2, 0,  ADC_CTL_CH10);
    ADCSequenceStepConfigure(ADC1_BASE, 2, 1,  ADC_CTL_CH11 | ADC_CTL_IE | ADC_CTL_END);
    
    // Enable hardware oversampling (higher precision results)
    ADCHardwareOversampleConfigure( ADC0_BASE , HW_OVERSAMPLING_FACTOR );
    ADCHardwareOversampleConfigure( ADC1_BASE , HW_OVERSAMPLING_FACTOR );
    
    // Load Timer5B with a frequency of ADC_SAMPLE_RATE * HW_OVERSAMPLING_FACTOR
    TimerLoadSet(TIMER5_BASE, TIMER_B, SysCtlClockGet() / ADC_SAMPLE_RATE / HW_OVERSAMPLING_FACTOR);
    
    // Enable Timer5B
    TimerEnable(TIMER5_BASE, TIMER_B);
}

void InitializeADC(tADC adc){
    static tBoolean isADCMainInitialized = false;
    if(!isADCMainInitialized){
        InitializeADCMain();
        isADCMainInitialized = true;
    }
    
    //Select the specified adc's pin mux to type adc
    GPIOPinTypeADC(ADCPortBase[adc], ADCGPIOPin[adc]);
    
    //Enable the sequencer for the specified adc and wait until the first accumulation finishes
    switch( adc/2 ){ // Two adc's per sequencer
        case 0: // Enable ADC0:Sequence 0
                ADCSequenceEnable(ADC0_BASE, 0);
                
                // Enable ADC0:Sequence 0 interrupt
                ADCIntEnable(ADC0_BASE, 0);
                
                // Busy-wait until the sequence finishes
                while(!ADCIntStatus(ADC0_BASE,0,false));
                
                // Enter the interrupt handler and complete initialization
                IntEnable(INT_ADC0SS0);
                
                break;
        case 1: // Enable ADC0:Sequence 1
                ADCSequenceEnable(ADC0_BASE, 1);
                
                // Enable ADC0:Sequence 1 interrupt
                ADCIntEnable(ADC0_BASE, 1);
                
                // Busy-wait until the sequence finishes
                while(!ADCIntStatus(ADC0_BASE,1,false));
                
                // Enter the interrupt handler and complete initialization
                IntEnable(INT_ADC0SS1);
                
                break;
        case 2: // Enable ADC0:Sequence 2
                ADCSequenceEnable(ADC0_BASE, 2);
                
                // Enable ADC0:Sequence 2 interrupt
                ADCIntEnable(ADC0_BASE, 2);
                
                // Busy-wait until the sequence finishes
                while(!ADCIntStatus(ADC0_BASE,2,false));
                
                // Enter the interrupt handler and complete initialization
                IntEnable(INT_ADC0SS2);
                
                break;
        case 3: // Enable ADC1:Sequence 0
                ADCSequenceEnable(ADC1_BASE, 0);
                
                // Enable ADC1:Sequence 0 interrupt
                ADCIntEnable(ADC1_BASE, 0);
                
                // Busy-wait until the sequence finishes
                while(!ADCIntStatus(ADC1_BASE,0,false));
                
                // Enter the interrupt handler and complete initialization
                IntEnable(INT_ADC1SS0);
                
                break;
        case 4: // Enable ADC1:Sequence 1
                ADCSequenceEnable(ADC1_BASE, 1);
                
                // Enable ADC1:Sequence 1 interrupt
                ADCIntEnable(ADC1_BASE, 1);
                
                // Busy-wait until the sequence finishes
                while(!ADCIntStatus(ADC1_BASE,1,false));
                
                // Enter the interrupt handler and complete initialization
                IntEnable(INT_ADC1SS1);
                
                break;
        case 5: // Enable ADC1:Sequence 2
                ADCSequenceEnable(ADC1_BASE, 2);
                
                // Enable ADC1:Sequence 2 interrupt
                ADCIntEnable(ADC1_BASE, 2);
                
                // Busy-wait until the sequence finishes
                while(!ADCIntStatus(ADC1_BASE,2,false));
                
                // Enter the interrupt handler and complete initialization
                IntEnable(INT_ADC1SS2);
                
                break;
    }
    
}

// Returns the latest value read from the specified ADC pin
// \param index is the ADC pin to read
// \return latest value read from the specified ADC pin
unsigned long GetADC(tADC adc) { 
    static tBoolean isADCInitialized[ADC_BUFFER_SIZE] = {false,};
    if(!isADCInitialized[adc]){
        InitializeADC(adc);
        isADCInitialized[adc] = true;
    }
    return g_rgADCValues[adc];
}

// Interrupt handler for ADC0:Sequencer 0
void ADC0SS0Handler(void){
    // Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC0_BASE, 0, &g_rgADCValues[ADC0]);
    
    // Clear the interrupt
    ADCIntClear(ADC0_BASE, 0);
}

// Interrupt handler for ADC0:Sequencer 1
void ADC0SS1Handler(void){
    // Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC0_BASE, 1, &g_rgADCValues[ADC2]);
    
    // Clear the interrupt
    ADCIntClear(ADC0_BASE, 1);
}

// Interrupt handler for ADC0:Sequencer 2
void ADC0SS2Handler(void){
    // Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC0_BASE, 2, &g_rgADCValues[ADC4]);
    
    // Clear the interrupt
    ADCIntClear(ADC0_BASE, 2);
}

// Interrupt handler for ADC1:Sequencer 0
void ADC1SS0Handler(void){
    // Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC1_BASE, 0, &g_rgADCValues[ADC6]);
    
    // Clear the interrupt
    ADCIntClear(ADC1_BASE, 0);
}

// Interrupt handler for ADC1:Sequencer 1
void ADC1SS1Handler(void){
    // Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC1_BASE, 1, &g_rgADCValues[ADC8]);
    
    // Clear the interrupt
    ADCIntClear(ADC1_BASE, 1);
}

// Interrupt handler for ADC1:Sequencer 2
void ADC1SS2Handler(void){
    // Put the new ADC readings into a global container
    ADCSequenceDataGet(ADC1_BASE, 2, &g_rgADCValues[ADC10]);
    
    // Clear the interrupt
    ADCIntClear(ADC1_BASE, 2);
}


// Interrupt handler for Timer5B
void ADCTriggerHandler(void){
    // Trigger ADC0:Sequence1
    ADCProcessorTrigger(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC0_BASE, 1);
    ADCProcessorTrigger(ADC0_BASE, 2);
    ADCProcessorTrigger(ADC1_BASE, 0);
    ADCProcessorTrigger(ADC1_BASE, 1);
    ADCProcessorTrigger(ADC1_BASE, 2);
    
    // Clear the interrupt
    TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
}
