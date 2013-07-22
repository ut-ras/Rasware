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
    
    // Pin independent tick count
    unsigned long tick;
    
    // Period of the signal
    unsigned long period;
    
    // Tick number to set states
    unsigned long on;
    unsigned long off;
};

// Buffer of pwm structs to use
// There may potentially be a pwm signal on every pin
tPWM pwmBuffer[PIN_COUNT];

int pwmCount = 0;


// Global initialization function called internally
// To setup shared interrupts
static InitializeGlobalPWM(void) {
    // flag for initialization
    static tBoolean initialized = false;
    
    // Check if we have already initialized
    if (initialized)
        return;
    
    // Enable the timer 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
  
    // We only need half a timer, so keep the configuration of the second half
    TimerConfigure(TIMER5_BASE, TIMER5_CFG_R | 
                                TIMER_CFG_SPLIT_PAIR | 
                                TIMER_CFG_B_PERIODIC);
  
    // Set timer B to run every `tick' as defined by resolution
    TimerLoadSet(TIMER5_BASE, TIMER_B, SysCtlClockGet() / PWM_RESOLUTION);
    
    // Enable the timer and interrupt
    TimerIntEnable(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
    IntEnable(INT_TIMER5B);
    TimerEnable(TIMER5_BASE, TIMER_B);
    
    initialized = true;
}


// Function to initialize pwm on a pin
// The returned pointer can be used by the SetPWM function
// Frequency must be specified in hertz
tPWM *InitializePWM(tPin pin, float freq) {
    tPWM *pwm;
    
    // Make sure pwm is initialized
    InitializeGlobalPWM();
    
    // Grab the next pwm
    pwm = &pwmBuffer[pwmCount];
    
    // Setup the initial data with frequency as given
    // zero phase and 50% duty cycle
    pwm->pin = pin;
    pwm->period = (unsigned long)(PWM_RESOLUTION / freq);
    pwm->on = 0;
    pwm->off = pwm->period / 2;
    
    // Prepare to write to pin
    GPIOPinTypeGPIOOutput(PORT_VAL(pin), PIN_VAL(pin));
    
    // Increment after we are setup to avoid race conditions
    pwmCount++;
    
    // Return the new pwm
    return pwm;
}

// In the handler we need to go through each pwm signal,
// adjust the tick counter in each to the new value,
// and update pins if they match the tick count
void Timer5BHandler(void) {
    int i;
    
    TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
    
    // Iterate over all pwm signals
    for (i=0; i < pwmCount; i++) {
        tPWM *pwm = &pwmBuffer[i];
        
        // Update tick counter
        pwm->tick = (pwm->tick + 1) % pwm->period;
        
        // Check if pin needs to be changed.
        // Check for `off' first, because if duty is set to 0, 
        // we don't want to turn on the pin
        if (pwm->tick == pwm->off) {
            GPIOPinWrite(PORT_VAL(pwm->pin), PIN_VAL(pwm->pin), 0x00);
        } else if (pwm->tick == pwm->on) {
            GPIOPinWrite(PORT_VAL(pwm->pin), PIN_VAL(pwm->pin), 0xff);
        }
    }
}

// This function sets a pwm duty cycle and phase
// Both Duty Cycle and Phase must be in percentage
void SetPWM(tPWM *pwm, float duty, float phase) {
    // First set the `on' time to be phase * period ticks
    pwm->on = (unsigned long)(phase * pwm->period);
    
    // Then set the `off' time to be duty * period ticks 
    // added to the calculated `on' time
    pwm->off = pwm->on + (unsigned long)(duty * pwm->period);
}

// This function sets a pwm frequency
// Frequency must be specified in hertz
void SetPWMFrequency(tPWM *pwm, float freq) {
    // First find the duty and phase for later
    // Phase is on time / period
    float phase = pwm->on / (float)pwm->period;
    // Duty Cycle is (off - on) / period
    float duty = (pwm->off - pwm->on) / (float)pwm->period;
    
    // Set the period to 1/freq, and multiply by resolution
    // for number of individual ticks
    pwm->period = (unsigned long)(PWM_RESOLUTION / freq);
    
    // Set the phase and duty with the new period
    SetPWM(pwm, duty, phase);
}
