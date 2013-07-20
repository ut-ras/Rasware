//*****************************************************************************
//
// pwm.h - software pwm drivers for the TLE5205-2
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

#include "gpio.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#ifndef __PWM_H__
#define __PWM_H__

// Compile time PWM variables
// For efficiency resolution must be a power of 2
#define PWM_RESOLUTION 4096
#define PWM_RATE 20

// Definition of struct PWM in pwm.c
typedef struct PWM tPWM;

// Function to initialize pwm on a pin
// The returned pointer can be used by the SetPWM function
tPWM *InitializePWM(tPin pin);

// This function sets a pwm duty cycle
void SetPWM(tPWM *pwm, float duty);

#endif //  __PWM_H__
