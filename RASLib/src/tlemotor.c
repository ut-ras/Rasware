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

#include "pwm.h"


// Definition of struct Motor
typedef struct TLEMotor {
    // Motor 'abstract' function
    void (*SetMotor)(struct TLEMotor *mtr, float input);

    // PWM signals used by motors
    tPWM *pwm0;
    tPWM *pwm1;

    // True if braking is applied
    tBoolean brake;

    // Set to switch motor direction
    tBoolean invert;
} tTLEMotor;

// Buffer of motor structs to use
// There can only be the total count of pins/2 since each
// motor needs 2 pins
static tTLEMotor tleMotorBuffer[PIN_COUNT / 2];

static int tleMotorCount = 0;


// This function sets a motor speed
static void SetTLEMotor(tTLEMotor *mtr, float input) { 
    // Check the input range
    if (input > 1 || input < -1)
        return;
    
    // invert if set
    if (mtr->invert)
        input *= -1;

    // Operate the motor controller
    // Motor controller operation is specific 
    // to the TLE5205-2
    if (mtr->brake) {
        if (input < 0) {
            // CCW (P, ~P)
            SetPWM(mtr->pwm0, 1.0f+input, 0.0f);
            SetPWM(mtr->pwm1, -input, 1.0f+input);
        } else if (input > 0) {
            // CW (P, 0)
            SetPWM(mtr->pwm0, 1.0f-input, 0.0f);
            SetPWM(mtr->pwm1, 0.0f, 0.0f);
        } else {
            // S (1, 0)
            SetPWM(mtr->pwm0, 1.0f, 0.0f);
            SetPWM(mtr->pwm1, 0.0f, 0.0f);
        }
    } else {
        if (input < 0) {
            // CCW (P, 1)
            SetPWM(mtr->pwm0, 1.0f+input, 0.0f);
            SetPWM(mtr->pwm1, 1.0f, 0.0f);
        } else if (input > 0) {
            // CW (P, P)
            SetPWM(mtr->pwm0, 1.0f-input, 0.0f);
            SetPWM(mtr->pwm1, 1.0f-input, 0.0f);
        } else {
            // S (1, 1)
            SetPWM(mtr->pwm0, 1.0f, 0.0f);
            SetPWM(mtr->pwm1, 1.0f, 0.0f);
        }
    }
}

// Function to initialize a motor on a pair of pins
// The returned pointer can be used by the SetMotor function
tTLEMotor *_InitializeTLEMotor(tPin a, tPin b, tBoolean brake, tBoolean invert) {
    // Grab the next motor
    tTLEMotor *mtr = &tleMotorBuffer[tleMotorCount++];
    
    // Setup the initial data
    mtr->brake = brake;
    mtr->invert = invert;
    
    // Initialize pwm on both pins
    mtr->pwm0 = InitializePWM(a, 1600.0f);
    mtr->pwm1 = InitializePWM(b, 1600.0f);

    // Set parent methods
    mtr->SetMotor = SetTLEMotor;
    
    // Return the new motor
    return mtr;
}

