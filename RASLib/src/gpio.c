//*****************************************************************************
//
// gpio.c - GPIO interrupt handlers
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

#include "gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "internal.h"

// Port table is used externally to lookup port values
unsigned long portTable[PORT_COUNT] = {
    GPIO_PORTA_BASE,
    GPIO_PORTB_BASE,
    GPIO_PORTC_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTF_BASE
};

// Internally used struct containing interrupt data
typedef struct {
    tCallback callback;
    void *data;
} tPinTask;

// Array of all possible pin interrupts
tPinTask pinTaskBuffer[PIN_COUNT];

// Internally used macro for defining port interrupt handlers
#define PORT_HANDLER(PORT)                                      \
void Port##PORT##Handler(void) {                                \
    unsigned long i, status;                                    \
    status = GPIOPinIntStatus(GPIO_PORT##PORT##_BASE, true);    \
                                                                \
    for (i = 0; i < 8; i++){                                    \
        if (status & (0x1 << i)) {                              \
            tPinTask *task = &pinTaskBuffer[PIN_##PORT##0 | i]; \
            task->callback(task->data);                         \
        }                                                       \
    }                                                           \
    GPIOPinIntClear(GPIO_PORT##PORT##_BASE, status);            \
}

// Interrupt handlers for Ports A through F
PORT_HANDLER(A);
PORT_HANDLER(B);
PORT_HANDLER(C);
PORT_HANDLER(D);
PORT_HANDLER(E);
PORT_HANDLER(F);

// Initialize GPIO
void InitializeGPIO(void) {
    int i;
    
    // Just clear all handlers
    for (i=0; i<PIN_COUNT; i++)
        pinTaskBuffer[i].callback = Dummy;
    
    // Enable the peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        
    // Enable the interrupts
    // This shouldn't be nescessary but for some reason is...
    IntEnable(INT_GPIOA);
    IntEnable(INT_GPIOB);
    IntEnable(INT_GPIOC);
    IntEnable(INT_GPIOD);
    IntEnable(INT_GPIOE);
    IntEnable(INT_GPIOF);
}

// Get Pin value as a boolean
tBoolean GetPin(tPin pin) {
    // Setting pin direction is just a bit set and fairly trivial
    GPIOPinTypeGPIOInput(PORT_VAL(pin), PIN_VAL(pin));
    
    // Get the actual pin value
    return GPIOPinRead(PORT_VAL(pin), PIN_VAL(pin)) ? true : false;
}

// Set Pin value as a boolean
void SetPin(tPin pin, tBoolean val) {
    // Setting pin direction is just a bit set and fairly trivial
    GPIOPinTypeGPIOOutput(PORT_VAL(pin), PIN_VAL(pin));
    
    // Set the actual pin value
    GPIOPinWrite(PORT_VAL(pin), PIN_VAL(pin), val ? 0xff : 0x00);
}

// Set a pin into high impedance mode
void SetPinZ(tPin pin) {
    // Setting pin direction to input places it in high impedance mode
    GPIOPinTypeGPIOInput(PORT_VAL(pin), PIN_VAL(pin));
}

// Register a callback to be called when the pin's value changes, 
// the state of the pin can then be determined through the GetPin function.
void CallOnPin(tCallback callback, void *data, tPin pin) {
    tPinTask *task = &pinTaskBuffer[pin];

    // Stop the interrupt first to avoid a race condition
    GPIOPinIntDisable(PORT_VAL(pin), PIN_VAL(pin));
    task->callback = Dummy;
    
    // Make sure the pin is setup as an input
    GPIOPinTypeGPIOInput(PORT_VAL(pin), PIN_VAL(pin));
    
    // If a null pointer is passed then we just leave the Dummy
    // to unregister the callback
    if (callback) {
        task->data = data;
        task->callback = callback;
        
        // Setup the interrupts
        GPIOPinIntEnable(PORT_VAL(pin), PIN_VAL(pin));
        GPIOIntTypeSet(PORT_VAL(pin), PIN_VAL(pin), GPIO_BOTH_EDGES);
    }
}
