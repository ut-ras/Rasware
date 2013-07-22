//*****************************************************************************
//
// servo.c - software servo (rc pwm) driver
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

#include "servo.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

// Function to initialize a servo on a pin
// The returned pointer can be used by the SetPWM function
tServo *InitializeServo(tPin pin) {
    // Create the pwm signal at 50Hz
    tServo *servo = InitializePWM(pin, 50.0f);
    
    // Set it to the center point
    SetServo(servo, 0.5);
    
    // Pass on the servo
    return servo;
}

// Servo output is 50hz with 2.5% to 12.5% duty cycle, centered at 7.5%
// This function sets a servo value, with 0.0 being 2.5% and 1.0 being 12.5%
void SetServo(tServo *servo, float value) { 
    // Set pwm to proper value
    SetPWMDuty(servo, 0.025f + (value * 0.10f));
}
