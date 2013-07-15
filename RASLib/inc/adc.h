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

typedef enum{ADC0,ADC1,ADC2,ADC3,ADC4,ADC5,ADC6,ADC7,ADC8,ADC9,ADC10,ADC11,ADC_BUFFER_SIZE} tADC;
extern unsigned long g_rgADCValues[ADC_BUFFER_SIZE];
unsigned long GetADC(tADC adc);
