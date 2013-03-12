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
//    port being used for motor interface (set defines above to enable the selection of ports)
//
// By default initializes pins PC4-7 for use by PWM 
//
// Enable the peripherals used by the motors.
//
//***************************************************************************

#ifdef DEFAULTINIT
void InitializeMotors(tbool bLeftInvert, tbool bRightInvert) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  //Enable PWM on portC

//***************************************************
//	Configure GPIO pins for PWM output
//***************************************************
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);   // PC4 - H-bridge selection, TimerA
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);   // PC5 - PWM signal to motor-controller, TimerB
    
    GPIOPinConfigure(GPIO_PC6_WT1CCP0);   // PC6 - H-bridge selection, TimerA
    GPIOPinConfigure(GPIO_PC7_WT1CCP1);   // PC7 - PWM signal to motor-controller, TimerB

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);   // Enable Timers (Wide Timer 0 and 1)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
    
//****************************************************
//	Split wide timers into 32-bit timers
//****************************************************
    TimerConfigure(WTIMER0_BASE, WTIMER_CFG_SPLIT_PAIR|WTIMER_CFG_A_PWM);
    TimerConfigure(WTIMER1_BASE, WTIMER_CFG_SPLIT_PAIR|WTIMER_CFG_A_PWM);
    
    ulPeriod = 256 + 8;       // 256 steps plus dead-band
    ulDutyCycle = 132;        // 50% duty cycle

//*****************************************************
//	Set load and match for timer resets 
//  (Will toggle pin when match reached, resets to load)
//***************************************************** 
    TimerLoadSet(WTIMER0_BASE, TIMER_A, ulPeriod - 1);   // load reset value with Period
    TimerMatchSet(WTIMER0_BASE, TIMER_A, ulPeriod / 2);  // Load match value with 1/2 of Period

    TimerLoadSet(WTIMER_BASE1, TIMER_A, ulPeriod - 1);
    TimerMatchSet(WTIMER_BASE1, TIMER_A, ulPeriod / 2);\

//******************************************************
//	Enable timers once they are set up
//******************************************************
    TimerEnable(WTIMER_BASE0, TIMER_A);
    TimerEnable(WTIMER_BASE1, TIMER_A);
	TimerEnable(WTIMER_BASE0, TIMER_B);
	TimerEnable(WTIMER_BASE1, TIMER_B);
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
