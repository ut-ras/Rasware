//*****************************************************************************
//
// pwmmotor - PWM based motor (RC PWM) driver
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

// Definition of struct PWMMotor
typedef struct PWMMotor {
    // Motor 'abstract' function
    void (*SetMotor)(struct PWMMotor *mtr, float input);

    // Pointer to pwm module
    tPWM *pwm;

    // Set to switch motor direction
    tBoolean invert;
} tPWMMotor;

// Buffer of motor structs to use
// There can only be as many pwm signals as pins
static tPWMMotor pwmMotorBuffer[PIN_COUNT];

static int pwmMotorCount = 0;


// Servo output is 50hz with 3% to 12% duty cycle, centered at 7.5%
// This function sets a motor value, with -1 being 3% and 1 being 12%
static void SetPWMMotor(tPWMMotor *mtr, float value) { 
    // Bind to the valid range of inputs
    if (value > 1 || value < -1)
        return;

    // invert if set
    if (mtr->invert)
        value *= -1;
    
    // Set pwm to proper value
    SetPWM(mtr->pwm, 0.075f + (value * 0.045f), 0.0f);
}

// Function to initialize a motor on a pin
// The returned pointer can be used by the SetMotor function
tPWMMotor *_InitializePWMMotor(tPin pin, tBoolean invert) {
    // Grab the next motor
    tPWMMotor *mtr = &pwmMotorBuffer[pwmMotorCount++];

    // Setup initial data
    mtr->invert = invert;

    // Create the pwm signal at 50Hz
    mtr->pwm = InitializePWM(pin, 50.0f);
    // Set it to the center point
    SetPWMMotor(mtr, 0);
    
    // Set parent methods
    mtr->SetMotor = SetPWMMotor;

    // Pass on the module
    return mtr;
}
