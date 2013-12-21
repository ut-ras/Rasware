//*****************************************************************************
//
// motor - Provides polymorphism for motor implementations
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

#include "motor.h"

// Definition of struct TLEMotor in tlemotor.c
typedef struct TLEMotor tTLEMotor;

tTLEMotor *_InitializeTLEMotor(tPin a, tPin b, tBoolean brake, tBoolean invert);

// Definition of struct ServoMotor in servomotor.c
typedef struct ServoMotor tServoMotor;

tServoMotor *_InitializeServoMotor(tPin pin, tBoolean invert);

// Definition of struct Motor
// Defined to tMotor in motor.h
struct Motor {
    void (*SetMotor)(tMotor *mtr, float value);
};


/**
 * Initializes a motor on a pair of pins for the TLE5205-2
 * @param a Pin that should plug into the IN1 motor line
 * @param b Pin that should plug into the IN2 motor line
 * @param brake Flag to enable breaking when the motor is set to 0 speed
 * @param invert Flag to switch the direction that the motor will turn
 * @return Pointer to an initialized tMotor, can be used by the SetMotor function
 */
tMotor *InitializeTLEMotor(tPin a, tPin b, tBoolean brake, tBoolean invert) {
    return (tMotor *)_InitializeTLEMotor(a, b, brake, invert);
}

/**
 * Initializes a motor on a single pin using rc pwm input
 * @param pin PWM signal line to motor
 * @param invert Flag to switch the direction that the motor will turn
 * @return Pointer to an initialized tMotor, can be used by the SetMotor function
 */
tMotor *InitializeServoMotor(tPin pin, tBoolean invert) {
    return (tMotor *)_InitializeServoMotor(pin, invert);
}

/**
 * Sets a motor speed
 * @param mtr Pointer to an initialized tMotor, returned by InitializeMotor
 * @param speed Float on range [-1, 1] where -1 means maximum backward speed and 1 means maximum forward speed
 */
void SetMotor(tMotor *mtr, float speed) {
    mtr->SetMotor(mtr, speed);
}
