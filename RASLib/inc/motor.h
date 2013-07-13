//*****************************************************************************
//
// motor.h - software motor driver
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

#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_GENERATOR_RESOLUTION 5000
#define MOTOR_GENERATOR_RATE 20
#define MOTOR_FUNCTION_BUFFER_SIZE 4

typedef enum{BRAKE, COAST} tMotorMode;
typedef struct{ 
    unsigned long port0; 
    unsigned long pin0;
    unsigned long port1; 
    unsigned long pin1;
    signed long value0;
    signed long value1;
    tMotorMode mode;
    tBoolean active;
} tMotorFunction;
extern tMotorFunction rgMotorFunctions[MOTOR_FUNCTION_BUFFER_SIZE];

unsigned long AddMotorFunction( unsigned long port0, unsigned long pin0 , unsigned long port1, unsigned long pin1, tMotorMode );
void SetMotorPosition(unsigned long index, float input);
// macro to create a motor signal generator
// e.g.,
// AddMotor(Left,F,2,F,3,COAST)
// creates InitializeMotorLeft() and SetMotorLeft(unsigned char)
#define AddMotor(NAME,PORT0,PIN0,PORT1,PIN1,MODE) \
unsigned long NAME ## MotorSelect; \
void InitializeMotor ## NAME (void){   \
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO ## PORT0); \
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO ## PORT1); \
    NAME ## MotorSelect = AddMotorFunction(GPIO_PORT ## PORT0 ## _BASE, GPIO_PIN_ ## PIN0, GPIO_PORT ## PORT1 ## _BASE, GPIO_PIN_ ## PIN1, MODE); \
} \
void SetMotor ## NAME (float input){ \
    SetMotorPosition( NAME ## MotorSelect, input); \
}

#endif //  __MOTOR_H__
