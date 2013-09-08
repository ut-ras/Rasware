//*****************************************************************************
//
// pwm - Software PWM drivers
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

#ifndef _R_PWM_H_
#define _R_PWM_H_

#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


// Definition of struct PWM in pwm.c
typedef struct PWM tPWM;

// Function to initialize pwm on a pin
// The returned pointer can be used by the SetPWM function
// Frequency must be specified in hertz
// If the number of frequencies passes the number of available
// modules, which is currently 12, then a null pointer is returned
tPWM *InitializePWM(tPin pin, float freq);

// This function sets a pwm duty cycle and phase
// Both Duty Cycle and Phase must be in percentage
void SetPWM(tPWM *pwm, float duty, float phase);


#endif //  _R_PWM_H_
