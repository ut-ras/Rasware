//*****************************************************************************
//
// adc.h - analog to digital converter drivers
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

#define ADC_SAMPLE_RATE 2000
#define HW_OVERSAMPLING_FACTOR 64

#define ADC_SIZE 2 // Number of ADC pins  
#define ADC0 GPIO_PIN_2	// PE2
#define ADC1 GPIO_PIN_3 // PE3

void InitializeADC(void);
unsigned long GetADC(int index);

