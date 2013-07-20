//*****************************************************************************
//
// pwm.c - software pwm drivers for the TLE5205-2 
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

#include "pwm.h"
#include "gpio.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

// Definition of struct PWM
// Defined to tPWM in pwm.h
struct PWM {
    // Which pin is being driven
    tPin pin;
    
    // Cycle number to turn off
    unsigned long duty;
};

// Buffer of pwm structs to use
// There may potentially be a pwm signal on every pin
tPWM pwmBuffer[PIN_COUNT];

int pwmCount = 0;

// Cycle count
unsigned long cycle;


// Global initialization function called internally
// To setup shared interrupts
static InitializeGlobalPWM(void) {
    long cycleTicks;
    
    // flag for initialization
    static tBoolean initialized = false;
    
    // Check if we have already initialized
    if (initialized)
        return;
    
    // Enable the timer 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
  
    // We use the first half of the timer to set the pins high
    // The second half we use check for lowering pins
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR | 
                                 TIMER_CFG_A_PERIODIC | 
                                 TIMER_CFG_B_PERIODIC);    
  
    // Setup timer A to run every cycle
    // and timer B to fire cycle/resolution
    cycleTicks = SysCtlClockGet() / PWM_RATE;
    TimerLoadSet(TIMER5_BASE, TIMER_A, cycleTicks);
    TimerLoadSet(TIMER5_BASE, TIMER_B, cycleTicks / PWM_RESOLUTION);
    
    // Enable the timers and interrupts
    TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
    IntEnable(INT_TIMER5A);
    IntEnable(INT_TIMER5B);
    TimerEnable(TIMER5_BASE, TIMER_BOTH);
    
    initialized = true;
}


// Function to initialize pwm on a pin
// The returned pointer can be used by the SetPWM function
tPWM *InitializePWM(tPin pin) {
    tPWM *pwm;
    
    // Make sure pwm is initialized
    InitializeGlobalPWM();
    
    // Grab the next pwm
    pwm = &pwmBuffer[pwmCount];
    
    // Setup the initial data
    pwm->pin = pin;
    pwm->duty = PWM_RESOLUTION / 2;
    
    // Prepare to write to pin
    GPIOPinTypeGPIOOutput(PORT_VAL(pin), PIN_VAL(pin));
    
    // Increment after we are setup to avoid race conditions
    pwmCount++;
    
    // Return the new pwm
    return pwm;
}

// Every full cycle reset the pins to the high position
void Timer5AHandler(void) {
    int i;
    
    TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
    
    // Set all active pins high
    for (i=0; i < pwmCount; i++) {
        tPin pin = pwmBuffer[i].pin;
        GPIOPinWrite(PORT_VAL(pin), PIN_VAL(pin), PIN_VAL(pin));
    }
}

// Check for any matches with the cycle count to set low
void Timer5BHandler(void) {
    int i;
    
    TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
    
    // Update the cycle count
    cycle = (cycle+1) & (PWM_RESOLUTION-1);
    
    // Check for any pins to set low
    for (i=0; i < pwmCount; i++) {
        if (pwmBuffer[i].duty == cycle) {
            tPin pin = pwmBuffer[i].pin;
            GPIOPinWrite(PORT_VAL(pin), PIN_VAL(pin), 0);
        }
    }
}

// This function sets a pwm duty cycle
void SetPWM(tPWM *pwm, float duty) {
    // Just set the cycle to turn off
    pwm->duty = duty * PWM_RESOLUTION;
}
