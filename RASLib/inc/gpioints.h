//*****************************************************************************
//
// gpioints.h - GPIO interrupt handlers
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

#define GPIO_INT_HANDLER_BUFFER_SIZE 4
typedef enum{PORTA,PORTB,PORTC,PORTD,PORTE,PORTF,NUM_GPIO_PORTS} tGPIO_Port;
typedef void(*tInterruptHandler)(void);

void AttachToInterruptHandler(tGPIO_Port port, tInterruptHandler handler);
void InitializeGPIOInterruptHandlers(void);

