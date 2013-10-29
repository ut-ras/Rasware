//*****************************************************************************
//
// gpiolinesensor - GPIO based line sensor
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

#ifndef _R_GPIOLINESENSOR_H_
#define _R_GPIOLINESENSOR_H_

#include "gpio.h"
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif


// Definition of struct GPIOLineSensor in gpiolinesensor.c
typedef struct GPIOLineSensor tGPIOLineSensor;

/**
 * Initializes a GPIO line sensor on the provided pins
 * @param p0 Pin plugged into sensor 0
 * @param p1 Pin plugged into sensor 1
 * @param p2 Pin plugged into sensor 2
 * @param p3 Pin plugged into sensor 3
 * @param p4 Pin plugged into sensor 4
 * @param p5 Pin plugged into sensor 5
 * @param p6 Pin plugged into sensor 6
 * @param p7 Pin plugged into sensor 7
 * @return Pointer to an initialized tGPIOLineSensor, can be used by the GPIOLineSensorRead functions
 */
tGPIOLineSensor *hiddenInitializeGPIOLineSensor(tPin p0, tPin p1, tPin p2, tPin p3, tPin p4, tPin p5, tPin p6, tPin p7);

/**
 * Returns the line sensor value measured as a bit-packed byte
 * @param ls Pointer to an initialized tGPIOLineSensor, returned by InitializeGPIOLineSensor
 * @param threshold If the value read from a single IR on the linesensor is above this threshold, it is converted to 1 and then packed into the byte
 * @return Value measured from the line sensors as a bit-packed byte, where each bit of the byte corresponds to the value of a single IR senor on the line sensor array
 * Note: if the line sensor is not continously reading, then the function will busy wait for the results
 */
unsigned char GPIOLineSensorRead(tGPIOLineSensor *ls, float threshold);

/**
 * Puts the values read from the line sensor into an array of 8 floats
 * @param ls Pointer to an initialized tGPIOLineSensor, returned by InitializeGPIOLineSensor
 * @param array Array of 8 percentages, each corresponding to an IR sensor in the line sensor array
 * Note: if the line sensor is not continously reading, then the function will busy wait for the results
 */
tBoolean GPIOLineSensorReadArray(tGPIOLineSensor *ls, float *array);

/**
 * Sets up a GPIO line sensor to be run in the background
 * @param snr Pointer to an initialized tGPIOLineSensor, returned by InitializeGPIOLineSensor
 * @param callback Function called the next time the line sensor read completes, in which a call to GPIOLineSensorRead will return with the newly obtained value immediately
 * @param data Argument sent to the provided callback function whenever it is called
 */
void GPIOLineSensorBackgroundRead(tGPIOLineSensor *ls, tCallback callback, void *data);

/**
 * Sets up an GPIO line sensor to be read indefinitly
 * @param snr Pointer to an initialized tGPIOLineSensor, returned by InitializeGPIOLineSensor
 * @param us Time between calls to read the line sensor in microseconds
 * Note: Any following calls to GPIOLineSensorRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the line sensor read to complete, the line sensor will fire as fast as possible without overlap
 */
void GPIOLineSensorReadContinuouslyUS(tGPIOLineSensor *ls, tTime us);

/**
 * Sets up an line sensor to be read indefinitly
 * @param snr Pointer to an initialized tGPIOLineSensor, returned by InitializeGPIOLineSensor
 * @param s Time between calls to read the line sensor in seconds
 * Note: Any following calls to GPIOLineSensorRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the line sensor read to complete, the line sensor will fire as fast as possible without overlap
 */
void GPIOLineSensorReadContinuously(tGPIOLineSensor *ls, float s);


#ifdef __cplusplus
}
#endif

#endif // _R_GPIOLINESENSOR_H_
