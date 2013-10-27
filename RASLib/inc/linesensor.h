//*****************************************************************************
//
// linesensor - I2C based line sensor
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
// Suppressed to be replaced with non I2C sensor

// // Definition of struct LineSensor in linesensor.c
// typedef struct LineSensor tLineSensor;

// /**
//  * Initializes a line sensor with an address on an i2c bus
//  * @param i2c Pointer to an initialized tI2C, returned by InitializeI2C
//  * @param address 2-bit value determined by the solder jumpers on the board
//  * @return Pointer to an initialized tLineSensor, can be used by the LineSensorRead functions
//  */
// tLineSensor *InitializeLineSensor(tI2C *i2c, unsigned int address);

// /**
//  * Returns the line sensor value measured as a bit-packed byte
//  * @param ls Pointer to an initialized tLineSensor, returned by InitializeLineSensor
//  * @param threshold If the value read from a single IR on the linesensor is above this threshold, it is converted to 1 and then packed into the byte
//  * @return Value measured from the line sensors as a bit-packed byte, where each bit of the byte corresponds to the value of a single IR senor on the line sensor array
//  * Note: if the line sensor is not continously reading, then the function will busy wait for the results
//  * Note: if there is an error in the I2C module, the returned value will be all ones
//  */
// unsigned char LineSensorRead(tLineSensor *ls, float threshold);

// /**
//  * Puts the values read from the line sensor into an array of 8 floats
//  * @param ls Pointer to an initialized tLineSensor, returned by InitializeLineSensor
//  * @param array Array of 8 percentages, each corresponding to an IR sensor in the line sensor array
//  * @return true if successful, otherwise it returns false and fills the array with infinities
//  * Note: if the line sensor is not continously reading, then the function will busy wait for the results
//  */
// tBoolean LineSensorReadArray(tLineSensor *ls, float *array);

// /**
//  * Sets up a line sensor to be run in the background
//  * @param snr Pointer to an initialized tLineSensor, returned by InitializeLineSensor
//  * @param callback Function called the next time the line sensor read completes, in which a call to LineSensorRead will return with the newly obtained value immediately
//  * @param data Argument sent to the provided callback function whenever it is called
//  */
// void LineSensorBackgroundRead(tLineSensor *ls, tCallback callback, void *data);

// /**
//  * Sets up an line sensor to be read indefinitly
//  * @param snr Pointer to an initialized tLineSensor, returned by InitializeLineSensor
//  * @param us Time between calls to read the line sensor in microseconds
//  * Note: Any following calls to LineSensorRead will return the most recent value
//  * Note: If the passed time between calls is less than the time it takes for the line sensor read to complete, the line sensor will fire as fast as possible without overlap
//  */
// void LineSensorReadContinuouslyUS(tLineSensor *ls, tTime us);

// /**
//  * Sets up an line sensor to be read indefinitly
//  * @param snr Pointer to an initialized tLineSensor, returned by InitializeLineSensor
//  * @param s Time between calls to read the line sensor in seconds
//  * Note: Any following calls to LineSensorRead will return the most recent value
//  * Note: If the passed time between calls is less than the time it takes for the line sensor read to complete, the line sensor will fire as fast as possible without overlap
//  */
// void LineSensorReadContinuously(tLineSensor *ls, float s);

// Pololu Line Sensor code from RASLib2012

#define NUM_SENSORS (8)                                        // the number of sensors
typedef unsigned char linesensor_t;                // the line sensor data type

// Summary: Initializes the appropriate GPIO
// Note:        Always call this function before any other sensor-related functions
//                        Initializes dischargeTime to a default value of 1000 us

extern void InitializeLineSensor(void);
// Summary:        Same as above, but sets up the line sensor for asynchronous operation.
//                         This means the line sensor will update automatically in the background
// Note:        Avoid mixing both normal and asynchronous operation
// Note:        Uses TIMER0. If you use asynchronous line sensing, you should not use TIMER0 elsewhere in your code;
//                        If you use TIMER0 in your code, you should not use asynchronous line sensing.
extern void InitializeLineSensorAsync(void);

// Summary: Sets the discharge time
// Parameters:
//                dischargeTime: the new discharge time 
//                                constraints: 120 <= dischargeTime <= 2600
// Note:        This function applies to both normal and asynchronous operation
extern void SetDischargeTime(unsigned long dischargeTime);

// Summary:        Reads the current line sensor values
// Note:        this method takes 10 us + dischargeTime to execute
// Returns:        a package of bits corresponding to the current readings from the line sensor
extern void ReadLineSensor(char *ret);



#ifdef __cplusplus
}
#endif

#endif // _R_LINESENSOR_H_
