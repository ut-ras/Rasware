//*****************************************************************************
//
// linesensor.h - driver for I2C line sensor
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


// Internally inits I2C and gives an address for the line sensor
void InitializeLineSensor(void);

// Grabs line sensor data using i2c and dumps into LineSensor array. 
// Returns 1 if successful, 0 if encountered a timeout error 
//	(timeout probably means the line sensor isn't connected correctly)
int ReadLineSensorArray(unsigned char *data);

unsigned char ReadLineSensor(void);
