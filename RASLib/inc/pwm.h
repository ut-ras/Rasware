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

/**
 * Initializes a PWM signal for a pin
 * @param pin Pin that will output a PWM signal
 * @param freq Frequency of the signal, must be specified in hertz
 * @return Pointer to an initialized tPWM, can be used by the SetPWM function
 * Note: If the number of frequencies passes the number of available modules, which is currently 12, then a null pointer is returned
 */
tPWM *InitializePWM(tPin pin, float freq);

/**
 * Sets a PWM duty cycle and phase
 * @param pwm Initialized tPWM, returned by InitializePWM
 * @param duty Duty cycle of signal, specified as a percentage
 * @param phase Phase of signal, specified as a percentage
 */
void SetPWM(tPWM *pwm, float duty, float phase);


#endif //  _R_PWM_H_
