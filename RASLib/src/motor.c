//*****************************************************************************
//
// motor.c - software pwm drivers for the TLE5205-2 
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

#include "motor.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

// Definition of struct Motor
// Defined to tMotor in motor.h
struct Motor {
    // PWM signals used by motors
    tPWM *pwm0;
    tPWM *pwm1;

    // True if braking is applied
    tBoolean brake;
};

// Buffer of motor structs to use
// There can only be the total count of pins/2 since each
// motor needs 2 pins
tMotor motorBuffer[PIN_COUNT / 2];

int motorCount = 0;


// Function to initialize a motor on a pair of pins
// The returned pointer can be used by the SetMotor function
tMotor *InitializeMotor(tPin a, tPin b, tBoolean brake) {
    // Grab the next motor
    tMotor *mtr = &motorBuffer[motorCount++];
    
    // Setup the initial data
    mtr->brake = brake;
    
    // Initialize pwm on both pins
    mtr->pwm0 = InitializePWM(a, 20.0f);
    mtr->pwm1 = InitializePWM(b, 20.0f);
    
    // Return the new motor
    return mtr;
}

// This function sets a motor speed
void SetMotor(tMotor *mtr, float input) { 
    // Check the input range
    if(input > 1 || input < -1)
        return;
    
    // Operate the motor controller
    // Motor controller operation is specific 
    // to the TLE5205-2
    if (mtr->brake) {
        if (input < 0) {
            // CCW (P, ~P)
            SetPWM(mtr->pwm0, 1.0f+input, -input);
            SetPWM(mtr->pwm1, -input, 0.0f);
        } else if (input > 0) {
            // CW (P, 0)
            SetPWM(mtr->pwm0, input, 0.0f);
            SetPWM(mtr->pwm1, 0.0f, 0.0f);
        } else {
            // S (1, 0)
            SetPWM(mtr->pwm0, 1.0f, 0.0f);
            SetPWM(mtr->pwm1, 0.0f, 0.0f);
        }
    } else {
        if (input < 0) {
            // CCW (P, 1)
            SetPWM(mtr->pwm0, 1.0f+input, -input);
            SetPWM(mtr->pwm1, 1.0f, 0.0f);
        } else if (input > 0) {
            // CW (P, P)
            SetPWM(mtr->pwm0, input, 0.0f);
            SetPWM(mtr->pwm1, input, 0.0f);
        } else {
            // S (1, 1)
            SetPWM(mtr->pwm0, 1.0f, 0.0f);
            SetPWM(mtr->pwm1, 1.0f, 0.0f);
        }
    }
}
