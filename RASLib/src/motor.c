//*****************************************************************************
//
// motor.c - motor initializations and command functions
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of RASLib Rev0 of the RASWare2013 package.
//
// Website: ras.ece.utexas.edu
// Contact: president@ras.ece.utexas.edu
//
//*****************************************************************************

/*#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h" */
#include "init.h"
#include "motor.h"

//***************************************************************************
//                  GLOBALS

unsigned long ulPeriod;
unsigned long ulDutyCycle;
//***************************************************************************

//#define VARIABLEINIT       // Uncomment to allow parameter passing to enable different ports for motor use.
#define DEFAULTINIT          // Uncomment to initialize PC4-6 by default

//***************************************************************************
// Summary:	Initializes the appropriate PWMs for motor output
// Note: Always call this function before any other motor-related functions
// Inputs: booleans for motor direction inversion (set false, false for no inversion),
//    port being used for motor interface
//
// Enable the peripherals used by the motors.
//
//***************************************************************************

#ifdef DEFAULTINIT
void InitializeMotors(tbool bLeftInvert, tbool bRightInvert) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
    
    ulPeriod = 1000;          // 16-bit timer, max 65536
    ulDutyCycle = 250;        //(.25 * period)
}
#endif
    
#ifdef VARIABLEINIT	 
    void InitializeMotors(tbool bLeftInvert, tbool bRightInvert, motorPortSet portSetIn) {
        
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
        
    switch ( portSetIn ) {
    case SetD:
         
        break;
    case SetEF:

        break;
    case SetBC:

        break;
    case SetCDF:

        break;        
    } 
#endif
} 
