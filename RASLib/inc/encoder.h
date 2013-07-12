//*****************************************************************************
//
// encoder.h - encoder driver
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
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"

unsigned long AddEncoder(unsigned long ulPort, 
                         unsigned char ucPinA, 
                         unsigned char ucPinB,
                         unsigned long ulInterrupt);
// macro to simplify adding an encoder
// e.g.,
// InitalizeEncoder(F,2,3) sets a new encoder with PF2 = A and PF3 = B
#define InitializeEncoder(PORT,PINA,PINB) AddEncoder(GPIO_PORT ## PORT ## _BASE, PINA, PINB, INT_GPIO ## PORT)
