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

// The number of PWM modules is simply the number of 
// available 32bit timers
#define PWM_MODULE_COUNT 12

// Internally used struct representing
// a single PWM module
typedef struct PWMModule {
    // Constant StellarisWare values 
    // specific to each module
    const unsigned long BASE;
    const unsigned long PERIPH;
    const unsigned long INT;
    const unsigned long TIMER;
    unsigned long * const CFG_R;    
    
    // Value of period module is set at
    unsigned long period;
    
    // All we need is a single pwm signal 
    // to get at the cycle
    tPWM *entry;
} tPWMModule;


// Array of available modules
static tPWMModule modBuffer[PWM_MODULE_COUNT] = {
    {WTIMER0_BASE, SYSCTL_PERIPH_WTIMER0, INT_WTIMER0, TIMER_A, &WTIMER0_CFG_R},
    {WTIMER0_BASE, SYSCTL_PERIPH_WTIMER0, INT_WTIMER0, TIMER_B, &WTIMER0_CFG_R},
    {WTIMER1_BASE, SYSCTL_PERIPH_WTIMER1, INT_WTIMER1, TIMER_A, &WTIMER1_CFG_R},
    {WTIMER1_BASE, SYSCTL_PERIPH_WTIMER1, INT_WTIMER1, TIMER_B, &WTIMER1_CFG_R},
    {WTIMER2_BASE, SYSCTL_PERIPH_WTIMER2, INT_WTIMER2, TIMER_A, &WTIMER2_CFG_R},
    {WTIMER2_BASE, SYSCTL_PERIPH_WTIMER2, INT_WTIMER2, TIMER_B, &WTIMER2_CFG_R},
    {WTIMER3_BASE, SYSCTL_PERIPH_WTIMER3, INT_WTIMER3, TIMER_A, &WTIMER3_CFG_R},
    {WTIMER3_BASE, SYSCTL_PERIPH_WTIMER3, INT_WTIMER3, TIMER_B, &WTIMER3_CFG_R},
    {WTIMER4_BASE, SYSCTL_PERIPH_WTIMER4, INT_WTIMER4, TIMER_A, &WTIMER4_CFG_R},
    {WTIMER4_BASE, SYSCTL_PERIPH_WTIMER4, INT_WTIMER4, TIMER_B, &WTIMER4_CFG_R},
    {WTIMER5_BASE, SYSCTL_PERIPH_WTIMER5, INT_WTIMER5, TIMER_A, &WTIMER5_CFG_R},
    {WTIMER5_BASE, SYSCTL_PERIPH_WTIMER5, INT_WTIMER5, TIMER_B, &WTIMER5_CFG_R},
};

static int modCount = 0;


// Definition of struct PWMEvent
// which contains all info for a single signal change
typedef struct PWMEvent {
    // Which pin is being driven
    tPin pin;
    
    // State to put pin into
    unsigned char state;
    
    // Ticks until next event
    unsigned long timing;
    
    // Pointers for the double linked list
    struct PWMEvent *next;
    struct PWMEvent *prev;
} tPWMEvent;

// Definition of struct PWM
// defined to be tPWM in pwm.h
struct PWM {
    // The owning module
    tPWMModule *module;
    
    // Value of phase and duty
    unsigned long phase;
    unsigned long duty;
    
    // Events to use
    tPWMEvent up;
    tPWMEvent down;
};

// Buffer of pwms to use
// There may potentially be a pwm signal on every pin
static tPWM pwmBuffer[PIN_COUNT];

static int pwmCount = 0;


// Module initialization function called internally
static InitializedPWMModule(PWMModule *mod) {
    // Use either timer A (shift 0) or timer B (shift 8)
    int tshift = (mod->TIMER == TIMER_A) ? 0 : 8;
    
    // Enable the timer
    SysCtlPeripheralEnable(mod->PERIPH);
    
    // We only need half a timer, so keep the configuration of the second half
    // Configure the timer for one shot usage
    TimerConfigure(mod->BASE, TIMER_CFG_SPLIT_PAIR |
                              *mod->CFG_R |
                              (TIMER_TIMA_TIMEOUT << tshift));
    
    // Setup the timer to fire immediately
    TimerLoadSet(mod->BASE, mod->TIMER, 1);
    TimerEnable(mod->BASE, mod->TIMER);
    
    // Enable the interrupt
    IntEnable(mod->INT);
    TimerIntEnable(mod->BASE, TIMER_TIMA_TIMEOUT << tshift);
}


// Function to initialize pwm on a pin
// The returned pointer can be used by the SetPWM function
// Frequency must be specified in hertz
tPWM *InitializePWM(tPin pin, float freq) {
    tPWMModule *mod;
    tPWM *pwm;
    int i;
    unsigned long period;
    
    // Grab the next pwm
    pwm = &pwmBuffer[pwmCount++];
    
    // Setup the pwm events
    pwm->up.state = 0xff;
    pwm->down.state = 0x00;
    
    // Setup the pin
    GPIOPinTypeGPIOOutput(PORT_VAL(pin), PIN_VAL(pin));
    pwm->up.pin = pin;
    pwm->down.pin = pin;
    
    // Calculate period
    period = (unsigned long)(SysCtlClockGet() / freq);
    
    // Find a module with the given frequency
    for (i = 0; i < PWM_MODULE_COUNT; i++) {
        
        // If we don't find a module we need to make a new one
        if (i == modCount) {
            // Grab the next module
            mod = &modBuffer[modCount++];
            
            // Keep track of the period
            mod->period = period;
            
            // Assign the pwm signal at 50% duty cycle
            // because the modules needs values to iterate over
            pwm->phase = 0;
            pwm->duty = period / 2;
            pwm->up.timing = pwm->duty;
            pwm->down.timing = pwm->duty;
            
            // Create the initial cycle
            pwm->up.next = pwm->up.prev = &pwm->down;
            pwm->down.next = pwm->down.prev = &pwm->up;
            mod->entry = pwm;
            
            // Initialize
            InitializePWMModule(mod);
            
            // Return the running pwm
            return pwm;
        }
        
        // If we find a module with the period we're looking for, 
        // stick our pwm in it
        if (modBuffer[i].period == period) {
            // Grab the module
            mod = &modBuffer[i];
            
            // Assign the module with no duty cycle and 
            // a phase equal to the entry point
            pwm->phase = mod->entry->phase;
            pwm->duty = 0;
            pwm->up.timing = 0;
            pwm->down.timing = 0;
            
            // Create the tiny link and put it after the entry point
            pwm->up.next = &pwm->down;
            pwm->down.prev = &pwm->up;
            
            pwm->up.prev = mod->entry;
            pwm->up.prev->next = &pwm->up;
            
            pwm->down.next = mod->entry->next;
            pwm->down.next->prev = &pwm->down;
            
            // Set the a duty cycle of 50% for consistency
            SetPWM(pwm, 0.5, 0);
            
            // Return the running pwm
            return pwm;
        }
    }
    
    // If no module is found, we put the pwm back and 
    // just return a null for failure
    pwmCount--;
    return 0;
}

/*** TODO all implementation below here ***/1

// In the handler, we need to simply update the state and
// move to the next event and setup the new timer value
#define TIMER_HANDLER(MOD)                          \
void WTimer##MOD##Handler(void) {                   \
    tADC *adc = modBuffer[MOD].contQueue;           \
    unsigned long data[8];                          \
    unsigned long *d = data;                        \
                                                    \
    ADCIntClear(ADC##MOD##_BASE, 0);                \
    ADCSequenceDataGet(ADC##MOD##_BASE, 0, data);   \
                                                    \
    for (; adc != 0; adc = adc->next)               \
        adc->value = *d++;                          \
}

// In the handler we need to go through each pwm signal,
// adjust the tick counter in each to the new value,
// and update pins if they match the tick count
void Timer5BHandler(void) {
    tPWM *pwm, *end;
    
    TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
    
    // Iterate over all pwm signals
    end = &pwmBuffer[pwmCount];
    for (pwm = pwmBuffer; pwm != end; pwm++) {
        // Update tick counter
        pwm->tick--;
        
        // Check if pin needs to be changed.
        // Check for `off' first, because if duty is set to 0, 
        // we don't want to turn on the pin. Clear the ticks if it is
        // past the zero to avoid a division and save a few cycles.
        if (pwm->tick == pwm->off) {
            GPIOPinWrite(PORT_VAL(pwm->pin), PIN_VAL(pwm->pin), 0x00);
            
        } else if (pwm->tick == 0) {
            GPIOPinWrite(PORT_VAL(pwm->pin), PIN_VAL(pwm->pin), 0xff);
            
            pwm->tick = pwm->period;
        }
    }
}

// This function sets a pwm duty cycle and phase
// Both Duty Cycle and Phase must be in percentage
// Tick count will reset on count, allowing for pwm synchronization
void SetPWM(tPWM *pwm, float duty, float phase) {
    // Limit the range of both values to [0.0,1.0]
    duty = (duty > 1.0f) ? 1.0f :
           (duty < 0.0f) ? 0.0f : duty;
    phase = (phase > 1.0f) ? 1.0f :
            (phase < 0.0f) ? 0.0f : phase;
    
    // Set the tick to given phase * period ticks
    // We then add it to the period as we are counting down
    // to cause a delay
    pwm->tick = pwm->period + (unsigned long)(phase * pwm->period);
    // Then set the `off' time to be duty * period ticks
    // Subtract it from the period as we are counting down
    pwm->off = pwm->period - (unsigned long)(duty * pwm->period);
}
