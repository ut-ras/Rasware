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

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h" 
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "init.h"
#include "motor.h"

tBoolean hi;

//***************************************************************************
//                  GLOBALS

unsigned long ulPeriod;
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

//***************************************************************************
//	IN1		IN2		OUT	 	|			Coast	Brake
//	0		0		CW		|	CW		 PP		 P0
//	0		1		CCW		|	CCW		 P1      P(~P)
//	1		0		Brake	|   S		 11      10
//	1		1		Coast	|
//
//	By default, initializes motors to run in "Coast" mode
//***************************************************************************

#ifdef DEFAULTINIT
void InitializeMotors(tBoolean bLeftInvert, tBoolean bRightInvert) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  //Enable PWM on portC

//************************************************************
//	Split wide timers into 32-bit timers
//************************************************************
    TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);
    TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);
					  	
//************************************************************
//	Configure GPIO pins for PWM output
//************************************************************
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);   // PC4 - TimerA - IN1
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);   // PC5 - PWM signal to motor-controller, TimerB  - IN2
    
    GPIOPinConfigure(GPIO_PC6_WT1CCP0);   // PC6 - TimerA - IN1
    GPIOPinConfigure(GPIO_PC7_WT1CCP1);   // PC7 - PWM signal to motor-controller, TimerB - IN2

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);   // Enable Timers (Wide Timer 0 and 1)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
    
    ulPeriod = 256 + 8;       // 256 steps plus dead-band

//************************************************************
//	Set load and match for timer resets 
//  (Will toggle pin when match reached, resets at value in load)
//	Value in match will be changed in motor set function calls
//************************************************************ 
    TimerLoadSet(WTIMER0_BASE, TIMER_A, ulPeriod - 1);   // load reset value with Period
    TimerMatchSet(WTIMER0_BASE, TIMER_A, ulPeriod / 2);  // Load match value with 1/2 of Period

    TimerLoadSet(WTIMER0_BASE, TIMER_B, ulPeriod - 1);
    TimerMatchSet(WTIMER0_BASE, TIMER_B, ulPeriod / 2);

	TimerLoadSet(WTIMER1_BASE, TIMER_A, ulPeriod - 1);
	TimerMatchSet(WTIMER1_BASE, TIMER_A, ulPeriod / 2);

	TimerLoadSet(WTIMER1_BASE, TIMER_B, ulPeriod - 1);
	TimerMatchSet(WTIMER1_BASE, TIMER_B, ulPeriod / 2);

//************************************************************
//	Enable timers 
//************************************************************
    TimerEnable(WTIMER0_BASE, TIMER_A);
    TimerEnable(WTIMER1_BASE, TIMER_A);
	TimerEnable(WTIMER0_BASE, TIMER_B);
	TimerEnable(WTIMER1_BASE, TIMER_B);
}
#endif

//************************************************************
//	If VARIABLEINIT defined in definition sect [above],
//	Then allows parameter passing for port selection
//************************************************************    
#ifdef VARIABLEINIT	 
    void InitializeMotors(tbool bLeftInvert, tbool bRightInvert, motorPortSet portSetIn) {       
    switch ( portSetIn ) {
    case SetD:
        //caseD 
        break;
    case SetEF:
		//caseEF
        break;
    case SetBC:
		//caseBC
        break;
    case SetCDF:
	    //caseCDF
        break;        
    } 
}
#endif
//************************************************************ 

//************************************************************
// Summary: Sets specified motor's power 
//	Inputs: motor selection, power output
//	Outputs: void
//	Parameters:
//		- MOTOR_0 or MOTOR_1
//		- 0 for neutral, 127 full forward, -128 full reverse
//************************************************************
void SetMotorPower(motor_t motor, power_t power) {
	unsigned short	usPulseWidth = power + 128 + 4;	
	if(power == 0) {			// Neutral, coast by defualt. IN1/IN2 drive high 
		TimerMatchSet(motor, TIMER_A, ulPeriod + 1);  		// Resets before match value, remains high (LoadSet = ulPeriod - 1)
		TimerMatchSet(motor, TIMER_B, ulPeriod + 1);
	}
	if(power < 0) {			 	// CCW rotation (reverse), IN1 = PWM, IN2 = high (coast by default)
		TimerMatchSet(motor, TIMER_A, usPulseWidth);
		TimerMatchSet(motor, TIMER_B, ulPeriod + 1);		
	}
	if(power > 0) {		 		// CW rotation, IN1	= IN2 = PWM (coast by default)
	    TimerMatchSet(motor, TIMER_A, usPulseWidth);
		TimerMatchSet(motor, TIMER_B, usPulseWidth);
	}	
}

void SetMotorPowers(power_t power0, power_t power1) {
	SetMotorPower(MOTOR_0, power0);
	SetMotorPower(MOTOR_1, power1);
}

/* Removed due to symbol collision
int main(void) {
	InitializeMotors(false,false);
	SetMotorPowers(127,127);
}*/
