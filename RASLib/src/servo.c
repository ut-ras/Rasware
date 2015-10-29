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

#include "servo.h"
#include "pwm.h"


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

// Servo output is 50hz with 3% to 12% duty cycle, centered at 7.5%
// This function sets a servo value, with 0.0 being 3% and 1.0 being 12%
void SetServo(tServo *servo, float value) { 
    // Bind to the valid range of inputs
    if (value > 1.0f || value < 0.0f)
        return;
    
    // Set pwm to proper value
    SetPWM(servo, 0.03f + (value * 0.09f), 0.0f);
}
