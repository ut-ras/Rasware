//*****************************************************************************
//
// gpioints.c - GPIO interrupt handlers
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

#include "gpioints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

tInterruptHandler rfptHandlers[NUM_GPIO_PORTS][GPIO_INT_HANDLER_BUFFER_SIZE];

// Does nothing
void Dummy(void){}

// Generic interrupt handlers for Ports A through F
void PortAHandler(void){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[PORTA][i] == Dummy){
            continue;
        }
        else{
            rfptHandlers[PORTA][i]();
        }
    }
	GPIOPinIntClear(GPIO_PORTA_BASE, GPIOPinIntStatus(GPIO_PORTA_BASE, false));
}
void PortBHandler(void){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[PORTB][i] == Dummy){
            continue;
        }
        else{
            rfptHandlers[PORTB][i]();
        }
    }
	GPIOPinIntClear(GPIO_PORTB_BASE, GPIOPinIntStatus(GPIO_PORTB_BASE, false));
}
void PortCHandler(void){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[PORTC][i] == Dummy){
            continue;
        }
        else{
            rfptHandlers[PORTC][i]();
        }
    }
	GPIOPinIntClear(GPIO_PORTC_BASE, GPIOPinIntStatus(GPIO_PORTC_BASE, false));
}
void PortDHandler(void){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[PORTD][i] == Dummy){
            continue;
        }
        else{
            rfptHandlers[PORTD][i]();
        }
    }
	GPIOPinIntClear(GPIO_PORTD_BASE, GPIOPinIntStatus(GPIO_PORTD_BASE, false));
}
void PortEHandler(void){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[PORTE][i] == Dummy){
            continue;
        }
        else{
            rfptHandlers[PORTE][i]();
        }
    }
	GPIOPinIntClear(GPIO_PORTE_BASE, GPIOPinIntStatus(GPIO_PORTE_BASE, false));
}
void PortFHandler(void){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[PORTF][i] == Dummy){
            continue;
        }
        else{
            rfptHandlers[PORTF][i]();
        }
    }
	GPIOPinIntClear(GPIO_PORTF_BASE, GPIOPinIntStatus(GPIO_PORTF_BASE, false));
}

// Adds a function to be executed during the specified port's interrupt handler execution
void AttachToInterruptHandler(tGPIO_Port port, tInterruptHandler handler){
    int i;
    for(i = 0; i < GPIO_INT_HANDLER_BUFFER_SIZE; i++){
        if(rfptHandlers[port][i] == Dummy){
            rfptHandlers[port][i] = handler;
            return;
        }
        else if(rfptHandlers[port][i] == handler){
            return;
        }
    }
}


// Loads a dummy function into all entries of the interrupt handlers buffer
void InitializeGPIOInterruptHandlers(void){
    int i,j;
    for(i = 0; i < NUM_GPIO_PORTS; i++){
        for(j = 0; j < GPIO_INT_HANDLER_BUFFER_SIZE; j++){
            rfptHandlers[i][j]=Dummy;
        }
    }
}
