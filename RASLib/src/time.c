//*****************************************************************************
//
// time.c - real time based functions
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

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "time.h"
#include "gpioints.h"
#include "internal.h"

/***************** GLOBAL VARIABLES *****************/
unsigned long g_ulSystemTimeMS;
unsigned long g_ulSystemTimeSeconds;


/***************** BEGIN DEFINITION OF SYSTEM TIME *****************/

// Output: system time in seconds
unsigned long GetTime(void){ return g_ulSystemTimeSeconds; }

// Output: system time in microseconds
unsigned long GetTimeMS(void){ return g_ulSystemTimeMS; }

// Initializes a system timer with microsecond resolution    
void InitializeSystemTime(void){
    // Initialize globals
    g_ulSystemTimeMS = 0;
    g_ulSystemTimeSeconds = 0;
    
    // Set up the period for the SysTick timer.  The SysTick timer period will
    // be equal to the system clock divided by 1000, resulting in a period of 1 millisecond.
    SysTickPeriodSet(SysCtlClockGet()/MS_PER_SEC);
    
    // Enable the SysTick Interrupt.
    SysTickIntEnable();
    
    // Enable SysTick.
    SysTickEnable();
}

void SysTickHandler(void){
    g_ulSystemTimeMS++;
    if(g_ulSystemTimeMS >= MS_PER_SEC){ // wrap US around every second
        g_ulSystemTimeMS = 0;
        g_ulSystemTimeSeconds++;
    }
}
/***************** END DEFINITION OF SYSTEM TIME *****************/



/***************** BEGIN DEFINITION OF PERIODIC FUNCTION GENERATOR *****************/

#define PERIODIC_FUNCTION_BUFFER_SIZE 16
typedef struct{
	void(*function)(void); 
    unsigned long period;
} tPeriodicFunction;
tPeriodicFunction rgPeriodicFunctions[PERIODIC_FUNCTION_BUFFER_SIZE];


void InitializePeriodicFunctions(void)
{
    int i;
    // Initialize the periodic function buffer to point to dummies
    // (period of 0 means not active)
    for( i = 0; i < PERIODIC_FUNCTION_BUFFER_SIZE; i++){
        rgPeriodicFunctions[i].function = Dummy;
        rgPeriodicFunctions[i].period = 0;
    }
    
    // Enable SysCtrl for Timer5
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); 
    
    // Configure Timer5A to be periodic, maintaining the configuration for Timer5B
    TimerConfigure(TIMER5_BASE, TIMER5_CFG_R | TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	
    // Enable the Timer5A interrupt
    IntEnable(INT_TIMER5A);
    TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT );
    
    // Load Timer5A with a frequency of PERIODIC_FUNCTION_RATE
    TimerLoadSet(TIMER5_BASE, TIMER_A, SysCtlClockGet() / PERIODIC_FUNCTION_RATE);
    
    // Enable Timer5A
    TimerEnable(TIMER5_BASE, TIMER_A);
}

// Declares a function to be run at the specified frequency
// \param function_in is a pointer to the function to run
// \param freq is the freqency in hertz in which to run the function
//        Note: PERIODIC_FUNCTION_RATE >= freq >= 1
// \return 0 for success, error code otherwise
// 
// Note: Uses Timer5A. Do not use elsewhere is this function is called
int AddPeriodicFunction(void(*function)(void), unsigned long freq)
{
    static tBoolean fIsPerFuncInitialized = false;
    int i = 0;
    
    // Check to see if periodic functions are initialized. If not, initialize.
    if(!fIsPerFuncInitialized){
        InitializePeriodicFunctions();
        
        //  Flag as initialized
        fIsPerFuncInitialized = true;
    }
    
    // Check to see if the freqency input is valid
    if(freq <= 0 || freq > PERIODIC_FUNCTION_RATE) return 1;
    
    // Find the next availible spot in the buffer
    while(rgPeriodicFunctions[i].period != 0)
    {
        i++;
        if(i >= PERIODIC_FUNCTION_BUFFER_SIZE) return 1; // exit with error if buffer is full
	}
    
    // Put the new task in the buffer
    rgPeriodicFunctions[i].function = function;
    rgPeriodicFunctions[i].period = PERIODIC_FUNCTION_RATE / freq;
    return 0;
}

void PeriodicFunctionHandler(void)
{
    static unsigned long cPeriodicThreadTime = 0;
    int i = 0;
    
    // Increment a counter in terms of periodic thread time
    cPeriodicThreadTime++;
    
    // Iterate through the periodic functions
    while( i != PERIODIC_FUNCTION_BUFFER_SIZE && rgPeriodicFunctions[i].period != 0)
    {
        // Run the function if the time % it's period is 0
        if(cPeriodicThreadTime % rgPeriodicFunctions[i].period == 0){
            rgPeriodicFunctions[i].function();
        }
        i++;	
    }
    
    // Clear the interrupt
    TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
}
/***************** END DEFINITION OF PERIODIC FUNCTION GENERATOR *****************/



/***************** BEGIN DEFINITION OF BUSY WAITS *****************/

// Busy-waits a specified number of microseconds
// \param us is the number of microseconds to wait
// Note: Uses WTimer5. Do not use elsewhere is this function is called.
void WaitUS(unsigned long long us)
{							
    static tBoolean fIsWaitInitialized = false;
    
    // Check to see if busy waits are initialized. If not, initialize.
    if(!fIsWaitInitialized)
    {
        // Enable SysCtrl for WTimer5
        SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
        
        // Configure WTimer5 as a 64-bit one shot timer
        TimerConfigure(WTIMER5_BASE, TIMER_CFG_ONE_SHOT);
        
        // Enable the WTimer5 interrupt
        TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
        
        // Flag as initialized
        fIsWaitInitialized = true;	
    }
    
    // Load WTimer5 with the number of microseconds to busy-wait for
    TimerLoadSet64(WTIMER5_BASE, us * (SysCtlClockGet() / US_PER_SEC) );	
    
    // Enable WTimer5	
    TimerEnable(WTIMER5_BASE, TIMER_A);		 
    
    // Spin until WTimer5 times out
    while(!(TimerIntStatus(WTIMER5_BASE, TIMER_A) & TIMER_TIMA_TIMEOUT));
    
    // Disable WTimer5
    TimerDisable(WTIMER5_BASE, TIMER_BOTH);
    
    // Clear the timeout interrupt
	TimerIntClear(WTIMER5_BASE, TimerIntStatus(WTIMER5_BASE, TIMER_A));    
}

// Busy-waits a specified amount of milliseconds
// \param ms is the number of milliseconds to wait
// Note: Uses WTimer5. Do not use elsewhere is this function is called.
void WaitMS(unsigned long long ms)
{
    WaitUS(ms*US_PER_MS);
}

// Busy-waits a specified amount of seconds
// \param seconds is the number of seconds to wait
// Note: Uses WTimer5. Do not use elsewhere is this function is called.
void WaitS(unsigned long seconds)
{
    WaitUS(seconds*US_PER_SEC);
}

// Busy-waits a specified amount of seconds
// \param seconds is the number of seconds to wait
// Note: Uses WTimer5. Do not use elsewhere is this function is called.
void Wait(float seconds)
{
    WaitUS(seconds*US_PER_SEC);
}
/***************** END DEFINITION OF BUSY WAITS *****************/

