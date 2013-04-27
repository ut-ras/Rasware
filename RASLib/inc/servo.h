//*****************************************************************************
//
// servo.h - software servo driver
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
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#ifndef __SERVO_H__
#define __SERVO_H__

#define SERVO_GENERATOR_RESOLUTION 4000
#define SERVO_GENERATOR_RATE 50
#define SERVO_FUNCTION_BUFFER_SIZE 16
typedef struct{ 
    unsigned long port; 
    unsigned long pin; 
    unsigned long value;
} tServoFunction;
tServoFunction rgServoFunctions[SERVO_FUNCTION_BUFFER_SIZE];

unsigned long AddServoFunction( unsigned long port, unsigned long pin );

// macro to create a servo signal generator
// e.g.,
// AddServo(Arm,F,1)
// creates ArmServoInit() and ArmServoSet(unsigned char)
#define AddServo(NAME,PORT,PIN) \
unsigned long NAME ## ServoSelect; \
void NAME ## ServoInit(void){   \
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO ## PORT); \
    NAME ## ServoSelect = AddServoFunction(GPIO_PORT ## PORT ## _BASE, GPIO_PIN_ ## PIN); \
} \
void NAME ## ServoSet(unsigned char input){ \
    SetServoPosition( NAME ## ServoSelect, input); \
}

#endif //  __SERVO_H__
