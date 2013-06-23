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

#include <StellarisWare/inc/hw_types.h>
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/inc/hw_memmap.h>

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
extern tServoFunction rgServoFunctions[SERVO_FUNCTION_BUFFER_SIZE];

unsigned long AddServoFunction( unsigned long port, unsigned long pin );

void InitializeServoGenerator(void); 

void SetServoPosition(unsigned long index, unsigned char input);

void ServoGeneratorHandler(void);

#endif //  __SERVO_H__
