//*****************************************************************************
//
// adc - Analog to Digital converter drivers
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
// Contact: ut.ieee.ras@gmail.com
//
//*****************************************************************************

#ifndef _R_ADC_H_
#define _R_ADC_H_

#include "gpio.h"
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif


// Constant for ADC oversampling
// Can be 0, 2, 4, 8, 16, 32, 64
// Doubling the oversample doubles to time to read ADC
#define ADC_OVERSAMPLING_FACTOR 32

// Definition of struct ADC in adc.c
typedef struct ADC tADC;

// ADC is only supported on a limited number of pins in hardware
// The following pins are supported:
// PIN_B4
// PIN_B5
// PIN_D0
// PIN_D1
// PIN_D2
// PIN_D3
// PIN_E0
// PIN_E1
// PIN_E2
// PIN_E3
// PIN_E4
// PIN_E5
#define ADC_COUNT 12

// Function to initialize an ADC on a pin, if ADC
// is not supported in hardware on the given pin, 
// then a null pointer is returned
// The returned pointer can be used by the ADCRead functions
tADC *InitializeADC(tPin pin);

// This function returns the value measured as a percentage
// If the ADC is not continously reading,
// then the function will busy wait for the results
float ADCRead(tADC *adc);

// This function sets up an ADC to be run in the background
// A callback can be passed, in which a call to ADCRead 
// will return with the newly obtained value immediately
void ADCBackgroundRead(tADC *snr, tCallback callback, void *data);

// These function set up an ADC to read indefinitly
// Any following calls to ADCRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the ADC to complete, the ADC will read as fast as possible without overlap
void ADCReadContinuouslyUS(tADC *snr, tTime us);
void ADCReadContinuously(tADC *snr, float s);

#ifdef __cplusplus
}
#endif

#endif // _R_ADC_H_
