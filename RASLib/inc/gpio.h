//*****************************************************************************
//
// gpio.h - GPIO interrupt handlers
// This file exists to sanction multiple functions per GPIO interrupt handler
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

#ifndef __GPIOINTS_H__
#define __GPIOINTS_H__

#include "internal.h"

#include "driverlib/gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

// Definition of tPin. Pins are named `PIN_XY' 
// where X is the port letter and Y is the pin number
typedef enum {
    PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5, PIN_A6, PIN_A7,
    PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, PIN_B6, PIN_B7,
    PIN_C0, PIN_C1, PIN_C2, PIN_C3, PIN_C4, PIN_C5, PIN_C6, PIN_C7,
    PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7,
    PIN_E0, PIN_E1, PIN_E2, PIN_E3, PIN_E4, PIN_E5, PIN_E6, PIN_E7,
    PIN_F0, PIN_F1, PIN_F2, PIN_F3, PIN_F4, PIN_F5, PIN_F6, PIN_F7,
    PIN_COUNT, PORT_COUNT = PIN_COUNT >> 3,
} tPin;

// These macros can be used to extract the pin and port bit values
// for use in StellarisWare function calls.
#define PIN_VAL(p)  (0x1 << ((p) & 0x7))
#define PORT_VAL(p) (portTable[(p) >> 3])
// portTable is defined in gpio.c
extern unsigned long portTable[PORT_COUNT];


// Initialize GPIO
void InitializeGPIO(void);

// Get Pin value as a boolean
tBoolean GetPin(tPin pin);

// Set Pin value as a boolean
void SetPin(tPin pin, tBoolean val);

// Set a pin into high impedance mode
void SetPinZ(tPin pin);

// Register a callback to be called when the pin's value changes, 
// the state of the pin can then be determined through the GetPin function.
void CallOnPin(tCallback callback, void *data, tPin pin);


#endif // __GPIOINTS_H__
