//*****************************************************************************
//
// init.c - initializations
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
#include <StellarisWare/driverlib/rom.h>
#include <StellarisWare/driverlib/sysctl.h>
#include "init.h"
#include "time.h"
#include <StellarisWare/driverlib/fpu.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/utils/cmdline.h>

tCmdLineEntry g_sCmdTable[]; 

void InitializeMCU(void)
{
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    FPULazyStackingEnable();
    
    // Set the clocking to run from PLL, using external oscillator
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    
    //Initialize System Timer
    InitializeSystemTime();
    
    //Enable global interrupts
    IntMasterEnable();    
}

