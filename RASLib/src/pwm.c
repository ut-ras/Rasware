//*****************************************************************************
//
// pwm - Software PWM drivers
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

#include "pwm.h"

#include <StellarisWare/inc/hw_ints.h>
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/lm4f120h5qr.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/driverlib/timer.h>
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/sysctl.h>


// The number of PWM modules is simply the number of 
// available 32bit timers
#define PWM_MODULE_COUNT 10

// Internally used struct representing
// a single PWM module
typedef struct PWMModule {
    // Constant StellarisWare values 
    // specific to each module
    const unsigned long BASE;
    const unsigned long PERIPH;
    const unsigned long INT;
    const unsigned long TIMER;
    volatile unsigned long * const CFG_R;    
    
    // Value of period module is set at
    unsigned long period;

    // We only need to keep track of where we are
    struct PWMEvent *event;
} tPWMModule;


// Array of available modules
static tPWMModule modBuffer[PWM_MODULE_COUNT] = {
    {WTIMER0_BASE, SYSCTL_PERIPH_WTIMER0, INT_WTIMER0A, TIMER_A, &WTIMER0_CFG_R},
    {WTIMER0_BASE, SYSCTL_PERIPH_WTIMER0, INT_WTIMER0B, TIMER_B, &WTIMER0_CFG_R},
    {WTIMER1_BASE, SYSCTL_PERIPH_WTIMER1, INT_WTIMER1A, TIMER_A, &WTIMER1_CFG_R},
    {WTIMER1_BASE, SYSCTL_PERIPH_WTIMER1, INT_WTIMER1B, TIMER_B, &WTIMER1_CFG_R},
    {WTIMER2_BASE, SYSCTL_PERIPH_WTIMER2, INT_WTIMER2A, TIMER_A, &WTIMER2_CFG_R},
    {WTIMER2_BASE, SYSCTL_PERIPH_WTIMER2, INT_WTIMER2B, TIMER_B, &WTIMER2_CFG_R},
    {WTIMER3_BASE, SYSCTL_PERIPH_WTIMER3, INT_WTIMER3A, TIMER_A, &WTIMER3_CFG_R},
    {WTIMER3_BASE, SYSCTL_PERIPH_WTIMER3, INT_WTIMER3B, TIMER_B, &WTIMER3_CFG_R},
    {WTIMER4_BASE, SYSCTL_PERIPH_WTIMER4, INT_WTIMER4A, TIMER_A, &WTIMER4_CFG_R},
    {WTIMER4_BASE, SYSCTL_PERIPH_WTIMER4, INT_WTIMER4B, TIMER_B, &WTIMER4_CFG_R},

// Used by time.h
//    {WTIMER5_BASE, SYSCTL_PERIPH_WTIMER5, INT_WTIMER5A, TIMER_A, &WTIMER5_CFG_R},
//    {WTIMER5_BASE, SYSCTL_PERIPH_WTIMER5, INT_WTIMER5B, TIMER_B, &WTIMER5_CFG_R},
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

    // The absolute timing from the begining of each cycle
    unsigned long target;
    
    // Pointers for the double linked list
    struct PWMEvent *next;
    struct PWMEvent *prev;
} tPWMEvent;


// Definition of struct PWM
// defined to be tPWM in pwm.h
struct PWM {
    // The period of the signal
    unsigned long period;
    
    // Events to use
    tPWMEvent up;
    tPWMEvent down;
};

// Buffer of pwms to use
// There may potentially be a pwm signal on every pin
static tPWM pwmBuffer[PIN_COUNT];

static int pwmCount = 0;


// Module initialization function called internally
// Requires a pwm signal to use
static void InitializePWMModule(tPWMModule *mod, tPWM *pwm) {
    // Use either timer A (shift 0) or timer B (shift 8)
    int tshift = (mod->TIMER == TIMER_A) ? 0 : 8;

    // We take the period from the pwm signal
    mod->period = pwm->period;

    // Then we setup the initial cycle
    pwm->up.target = 0;
    pwm->up.timing = pwm->period / 2;
    pwm->down.target = pwm->period / 2;
    pwm->down.timing = pwm->period / 2;

    // Connect the linked list
    pwm->up.prev = pwm->up.next = &pwm->down;
    pwm->down.next = pwm->down.prev = &pwm->up;

    // And we set the start of the cycle
    mod->event = &pwm->up;
    
    
    // Enable the timer
    SysCtlPeripheralEnable(mod->PERIPH);
    
    // We only need half a timer, so keep the configuration of the second half
    // Configure the timer for one shot usage
    TimerConfigure(mod->BASE, TIMER_CFG_SPLIT_PAIR |
                              *mod->CFG_R |
                              (TIMER_TIMA_TIMEOUT << tshift));
    
    // Setup the timer
    TimerLoadSet(mod->BASE, mod->TIMER, pwm->up.timing);
    // This is a bit hacky and assumes the order of pwm modules
    // By editing the config register both timers are disabled
    TimerEnable(mod->BASE, mod->TIMER | TIMER_A);
    
    // Enable the interrupt
    IntEnable(mod->INT);
    TimerIntEnable(mod->BASE, TIMER_TIMA_TIMEOUT << tshift);
}


// Internally used helper function for inserting new pwm signals
static void InsertPWM(tPWMModule *mod, tPWM *pwm) {
    // Setup the pwm to have no duty cycle and phase equal
    // to the entry point so insertion will not affect the cycle
    tPWMEvent *entry = mod->event;

    pwm->up.target = entry->target;
    pwm->up.timing = 0;
    pwm->down.target = entry->target;
    pwm->down.timing = 0;

    // First we insert the pwm events into the entry point
    // without any timing associated with them
    // We insert it before as to not interfere with current operation
    pwm->up.prev = entry->prev;
    pwm->up.next = &pwm->down;
    pwm->down.prev = &pwm->up;
    pwm->down.next = entry;

    entry->prev->next = &pwm->up;
    entry->prev = &pwm->down;

    // Then we simply call SetPWM and let it entangle the
    // nodes appropriately
    SetPWM(pwm, 0.5f, 0.0f);
}    


// Function to initialize pwm on a pin
// The returned pointer can be used by the SetPWM function
// Frequency must be specified in hertz
// If the number of frequencies passes the number of available
// modules, which is currently 12, then a null pointer is returned
tPWM *InitializePWM(tPin pin, float freq) {
    tPWMModule *mod;
    tPWM *pwm;
    int i;
    
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
    pwm->period = (unsigned long)(SysCtlClockGet() / freq);
    
    // Find a module with the given frequency
    for (i = 0; i < PWM_MODULE_COUNT; i++) {
        
        // If we don't find a module we need to make a new one
        if (i == modCount) {
            // Grab the next module
            mod = &modBuffer[modCount++];

            // Initialize
            InitializePWMModule(mod, pwm);
            
            // Return the running pwm
            return pwm;
        }
        
        // If we find a module with the period we're looking for, 
        // stick our pwm in it
        if (modBuffer[i].period == pwm->period) {
            // Grab the module
            mod = &modBuffer[i];
            
            // Add the new signal to the running ones
            InsertPWM(mod, pwm);
            
            // Return the running pwm
            return pwm;
        }
    }
    
    // If no module is available, we put the pwm back and 
    // just return a null for failure
    pwmCount--;
    return 0;
}


// In the handler, we need to move to the next event, set the state, and
// setup the next timing value. We group any events together that have the
// same time value, as that occurs often in synchronized pwm
#define TIMER_HANDLER(BASE, TIM)                                                \
void WTimer##BASE##TIM##Handler(void) {                                         \
    tPWMModule *mod = &modBuffer[2*BASE + (TIMER_##TIM == TIMER_B)];            \
    tPWMEvent *event = mod->event;                                              \
                                                                                \
    TimerIntClear(WTIMER##BASE##_BASE, TIMER_TIM##TIM##_TIMEOUT);               \
                                                                                \
    do {                                                                        \
        event = event->next;                                                    \
                                                                                \
        GPIOPinWrite(PORT_VAL(event->pin), PIN_VAL(event->pin), event->state);  \
    } while (event->timing == 0);                                               \
                                                                                \
    mod->event = event;                                                         \
    TimerLoadSet(WTIMER##BASE##_BASE, TIMER_##TIM, event->timing);              \
    TimerEnable(WTIMER##BASE##_BASE, TIMER_##TIM);                              \
}

// Interrupt handlers for each timer
TIMER_HANDLER(0, A);
TIMER_HANDLER(0, B);
TIMER_HANDLER(1, A);
TIMER_HANDLER(1, B);
TIMER_HANDLER(2, A);
TIMER_HANDLER(2, B);
TIMER_HANDLER(3, A);
TIMER_HANDLER(3, B);
TIMER_HANDLER(4, A);
TIMER_HANDLER(4, B);

// Used in timer.h
//TIMER_HANDLER(5, A);
//TIMER_HANDLER(5, B);


// This is an internally used function to move an 
// event forward in a cycle. Must disentangle and
// reentangle itself in the doubly linked list 
static void MoveEventForward(tPWMEvent *event, unsigned long diff) {
    tPWMEvent *prev = event->prev;

    // Find the difference from the next node
    diff += event->timing;

    // Take it out of the list
    event->prev->timing += event->timing;
    event->next->prev = event->prev;
    event->prev->next = event->next;

    // Find out where it needs to go
    while (prev->timing < diff) {
        diff -= prev->timing;
        prev = prev->prev;
    }

    // Update its timing/position
    event->timing = diff;
    event->next = prev->next;
    event->prev = prev;

    // Insert it back into the list
    prev->timing -= diff;
    prev->next->prev = event;
    prev->next = event;
}

// This is an internally used function to move an 
// event backward in a cycle. Must disentangle and
// reentangle itself in the doubly linked list 
static void MoveEventBackward(tPWMEvent *event, unsigned long diff) {
    tPWMEvent *prev = event->prev;

    // Find the difference from the prev node
    diff += prev->timing;

    // Take it out of the list
    event->prev->timing += event->timing;
    event->next->prev = event->prev;
    event->prev->next = event->next;

    // Find out where it needs to go
    while (prev->timing < diff) {
        diff -= prev->timing;
        prev = prev->next;
    }

    // Update its timing/position
    event->timing = prev->timing - diff;
    event->next = prev->next;
    event->prev = prev;

    // Insert it back into the list
    prev->timing = diff;
    prev->next->prev = event;
    prev->next = event;
}


// This function sets a pwm duty cycle and phase
// Both Duty Cycle and Phase must be in percentage
void SetPWM(tPWM *pwm, float duty, float phase) {
    unsigned long iphase, iduty;

    // Limit the range of both values to [0.0,1.0]
    if (duty > 1.0f || duty < 0.0f)
        return;
    
    if (phase > 1.0f || phase < 0.0f)
        return;

    // Calculate the new absolute phase and duty
    iphase = (unsigned long)(phase * pwm->period);
    iduty = iphase + (unsigned long)(duty * pwm->period);

    // The order of the following movements in the cycle is important
    // to make sure the events don't overlap at 0% and 100% duty cycles

    // Push the events forward if nescessary
    if (iphase < pwm->up.target)
        MoveEventForward(&pwm->up, pwm->up.target - iphase);
    if (iduty < pwm->down.target)
        MoveEventForward(&pwm->down, pwm->down.target - iduty);

    // Push the events backward if nescessary
    if (iduty > pwm->down.target)
        MoveEventBackward(&pwm->down, iduty - pwm->down.target);
    if (iphase > pwm->up.target)
        MoveEventBackward(&pwm->up, iphase - pwm->up.target);

    // Update the new phase/duty values
    pwm->up.target = iphase;
    pwm->down.target = iduty;
}
