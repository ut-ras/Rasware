//*****************************************************************************
//
// servo.h - software servo (rc pwm) driver
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

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "internal.h"

#ifndef __SERVO_H__
#define __SERVO_H__

#define SERVO_GENERATOR_RESOLUTION 4000
#define SERVO_GENERATOR_RATE 50
#define SERVO_FUNCTION_BUFFER_SIZE 16

// macro to create a servo signal generator
// e.g.,
// AddServo(Arm,F,1)
// creates InitializeServoArm() and SetServoArm(unsigned char)
#define AddServo(NAME,PORT,PIN) \
unsigned long NAME ## ServoSelect; \
void InitializeServo ## NAME (void){   \
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO ## PORT); \
    NAME ## ServoSelect = AddServoFunction(GPIO_PORT ## PORT ## _BASE, GPIO_PIN_ ## PIN); \
} \
void SetServo ## NAME (unsigned char input){ \
    SetServoPosition( NAME ## ServoSelect, input); \
}

#endif //  __SERVO_H__
