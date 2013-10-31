//*****************************************************************************
//
// linesensor - Provides polymorphism for line sensor implementations
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

#ifndef _R_LINESENSOR_H_
#define _R_LINESENSOR_H_

#include "time.h"
#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LineSensor tLineSensor;

/**
 * Initializes a line sensor with an address on an i2c bus
 * @param i2c Pointer to an initialized tI2C, returned by InitializeI2C
 * @param address 2-bit value determined by the solder jumpers on the board
 * @return Pointer to an initialized tLineSensor, can be used by the LineSensorRead functions
 */
tLineSensor *InitializeI2CLineSensor(tI2C *i2c, unsigned int address);

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
 * @return Pointer to an initialized tLineSensor, can be used by the LineSensorRead functions
 */
tLineSensor *InitializeGPIOLineSensor(tPin p0, tPin p1, tPin p2, tPin p3, tPin p4, tPin p5, tPin p6, tPin p7);

/**
 * Returns the line sensor value measured as a bit-packed byte
 * @param ls Pointer to an initialized tLineSensor, returned by InitializeLineSensor
 * @param threshold If the value read from a single IR on the linesensor is above this threshold, it is converted to 1 and then packed into the byte
 * @return Value measured from the line sensors as a bit-packed byte, where each bit of the byte corresponds to the value of a single IR senor on the line sensor array
 * Note: if the line sensor is not continously reading, then the function will busy wait for the results
 * Note: if there is an error in the I2C module, the returned value will be all ones
 */
unsigned char LineSensorRead(tLineSensor *ls, float threshold);

/**
 * Puts the values read from the line sensor into an array of 8 floats
 * @param ls Pointer to an initialized tLineSensor, returned by InitializeLineSensor
 * @param array Array of 8 percentages, each corresponding to an IR sensor in the line sensor array
 * @return true if successful, otherwise it returns false and fills the array with infinities
 * Note: if the line sensor is not continously reading, then the function will busy wait for the results
 */
tBoolean LineSensorReadArray(tLineSensor *ls, float *array);

/**
 * Sets up a line sensor to be run in the background
 * @param snr Pointer to an initialized tLineSensor, returned by InitializeLineSensor
 * @param callback Function called the next time the line sensor read completes, in which a call to LineSensorRead will return with the newly obtained value immediately
 * @param data Argument sent to the provided callback function whenever it is called
 */
void LineSensorBackgroundRead(tLineSensor *ls, tCallback callback, void *data);

/**
 * Sets up an line sensor to be read indefinitly
 * @param snr Pointer to an initialized tLineSensor, returned by InitializeLineSensor
 * @param us Time between calls to read the line sensor in microseconds
 * Note: Any following calls to LineSensorRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the line sensor read to complete, the line sensor will fire as fast as possible without overlap
 */
void LineSensorReadContinuouslyUS(tLineSensor *ls, tTime us);

/**
 * Sets up an line sensor to be read indefinitly
 * @param snr Pointer to an initialized tLineSensor, returned by InitializeLineSensor
 * @param s Time between calls to read the line sensor in seconds
 * Note: Any following calls to LineSensorRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the line sensor read to complete, the line sensor will fire as fast as possible without overlap
 */
void LineSensorReadContinuously(tLineSensor *ls, float s);


#ifdef __cplusplus
}
#endif

#endif // _R_LINESENSOR_H_
