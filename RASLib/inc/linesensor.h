//*****************************************************************************
//
// encoder.h - encoder driver
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

#ifndef __LINESENSOR_H__
#define __LINESENSOR_H__

#include "time.h"
#include "i2c.h"

// Definition of struct LineSensor in linesensor.c
typedef struct LineSensor tLineSensor;

// Function to initialize a line sensor on a pair of pins
// Address is be a 2-bit value determined by the solder jumpers on the board
// The returned pointer can be used by the LineSensorRead functions
tLineSensor *InitializeLineSensor(tPin sda, tPin scl, unsigned int address);

// This function returns the values measured as a bit-packed byte
// each bit is matched against a threshold that is passed.
// If the LineSensor is not continously reading,
// then the function will busy wait for the results
unsigned char LineSensorRead(tLineSensor *ls, float threshold);

// This function returns the values measured as an
// array of ratios placed in the passed memory location.
// If the LineSensor is not continously reading,
// then the function will busy wait for the results
void LineSensorReadArray(tLineSensor *ls, float *array);

// This function sets up a LineSensor to be run in the background
// A callback can be passed, in which a call to LineSensorRead 
// will return with the newly obtained value immediately
void LineSensorBackgroundRead(tLineSensor *ls, tCallback callback, void *data);

// These function set up a LineSensor to read indefinitly
// Any following calls to LineSensorRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the LineSensor to complete, the LineSensor will read as fast as possible without overlap
void LineSensorReadContinuouslyUS(tLineSensor *ls, tTime us);
void LineSensorReadContinuously(tLineSensor *ls, float s);


#endif /* __LINESENSOR_H__ */
