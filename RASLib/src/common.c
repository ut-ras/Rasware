//*****************************************************************************
//
// common - Shared functionality
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

#include "common.h"
#include "time.h"
#include "gpio.h"
#include "uart.h"

#include <StellarisWare/inc/hw_types.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/fpu.h>
#include <StellarisWare/driverlib/interrupt.h>


// Does nothing
void Dummy(void) {}


// The following function sets up the LM4F to use RASLib
void InitializeMCU(void) {
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    FPULazyStackingEnable();
    
    // Set the clocking to run from PLL, using external oscillator
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    
    //Initialize System Timer
    InitializeSystemTime();
    
    //Initialize GPIO Interrupt Handlers
    InitializeGPIO();
    
    //Initialize UART for communication
    InitializeUART(115200);
    InitializeDoublePrintHack();
    
    //Enable global interrupts
    IntMasterEnable();    
}


// Default hard-fault handler sets led
static void DefaultFault(void) {
    SetPin(PIN_F1, true);
}

// Hard-fault handles
static tCallback faultCallback = DefaultFault;
static void *faultData;


// Called on hard-faults
void PanicHandler(void) {
    faultCallback(faultData);
}

