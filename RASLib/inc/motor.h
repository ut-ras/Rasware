//*****************************************************************************
//
// motor - Software PWM drivers for the TLE5205-2
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

#ifndef _R_MOTOR_H_
#define _R_MOTOR_H_

#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


// Definition of struct Motor in motor.c
typedef struct Motor tMotor;

// Function to initialize a motor on a pair of pins
// The returned pointer can be used by the SetMotor function
tMotor *InitializeMotor(tPin a, tPin b, tBoolean brake, tBoolean invert);

// This function sets a motor speed
void SetMotor(tMotor *mtr, float speed);


#ifdef __cplusplus
}
#endif

#endif // _R_MOTOR_H_
