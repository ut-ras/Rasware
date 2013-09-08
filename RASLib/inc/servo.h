//*****************************************************************************
//
// servo - Servo (RC PWM) driver
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

#ifndef _R_SERVO_H_
#define _R_SERVO_H_

#include "gpio.h"
#include "pwm.h"

#ifdef __cplusplus
extern "C" {
#endif


// All a servo needs is a pointer to a pwm
typedef tPWM tServo;

// Function to initialize a servo on a pin
// The returned pointer can be used by the SetPWM function
tServo *InitializeServo(tPin pin);

// Servo output is 50hz with 2.5% to 12.5% duty cycle, centered at 7.5%
// This function sets a servo value, with 0.0 being 2.5% and 1.0 being 12.5%
void SetServo(tServo *servo, float value);


#ifdef __cplusplus
}
#endif

#endif //  _R_SERVO_H_
