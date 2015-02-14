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

/**
 * Initializes an ADC on a pin
 * @param pin Pin plugged into a servo
 * @return Pointer to an initialized tADC, can be used by the ADCRead functions
 * Note: if ADC is not supported in hardware on the given pin, then a null pointer is returned
 */
tADC *InitializeADC(tPin pin);

/**
 * Returns the voltage on the pin provided to InitializeADC as a % of 3.3V. (%'s are always between 0.0 and 1.0) 
 * @param adc Pointer to an initialized tADC, returned by InitializeADC
 * @return Value measured as a percentage
 * Note: if the ADC is not continously reading, then the function will busy wait for the results
 */
float ADCRead(tADC *adc);

/**
 * Sets up an ADC to be run in the background
 * @param adc Pointer to an initialized tADC, returned by InitializeADC
 * @param callback Function called the next time the ADC updates, in which a call to ADCRead will return with the newly obtained value immediately
 * @param data Argument sent to the provided callback function whenever it is called
 */
void ADCBackgroundRead(tADC *adc, tCallback callback, void *data);

/**
 * Sets up an ADC to read indefinitly
 * @param adc Pointer to an initialized tADC, returned by InitializeADC
 * @param us Time between calls to read the ADC specified in micro seconds
 * Note: Any following calls to ADCRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the ADC to complete, the ADC will read as fast as possible without overlap
 */
void ADCReadContinuouslyUS(tADC *adc, tTime us);

/**
 * Sets up an ADC to read indefinitly
 * @param adc Pointer to an initialized tADC, returned by InitializeADC
 * @param s Time between calls to read the ADC specified in seconds
 * Note: Any following calls to ADCRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the ADC to complete, the ADC will read as fast as possible without overlap
 */
void ADCReadContinuously(tADC *adc, float s);


#ifdef __cplusplus
}
#endif

#endif // _R_ADC_H_
