//*****************************************************************************
//
// time - Real Time based functions
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

#include "time.h"

#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_ints.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/systick.h>
#include <StellarisWare/driverlib/timer.h>
#include <StellarisWare/driverlib/interrupt.h>


// Global System Clock
// Only contains resolution to sysTickPeriod
static volatile tTime systemTiming = 0;

// Timing information
static long ticksInUS;
static tTime sysTickTiming;

// Relevant task info
typedef struct Task {
    // Task identifier
    int id;
    
    // Target time value
    tTime target;
    
    // If repeating, then repeatTime 
    // holds the period
    tTime repeatTime;
    
    // Callback data
    void *data;
    tCallback callback;

    // Pointer to next task in linked list
    struct Task *next;
} tTask;


// Queue of tasks implemented as a linked list
static tTask *pendingQueue;

// Queue of unused tasks also a linked list
static tTask *unusedQueue;

// Buffer of tasks to use
static tTask taskBuffer[TASK_COUNT];

// Next identifier
static int nextID = 42;


// Initializes a system timer with microsecond resolution
void InitializeSystemTime(void) {
    int i;

    // Reset global clock
    systemTiming = 0;
  
    // Reset queue and thread all tasks in the buffer together
    unusedQueue = &taskBuffer[0];

    for (i = 0; i < TASK_COUNT-1; i++)
        taskBuffer[i].next = &taskBuffer[i+1];

    taskBuffer[TASK_COUNT-1].next = 0;

    // Reset the pending queue as well
    pendingQueue = 0;
  

    // Find the system clock divided by 1s, which results in 
    // 1 US for the timer
    ticksInUS = SysCtlClockGet() / US(1);

    // Find the time it SysTick takes in US
    sysTickTiming = SYSTICK_PERIOD / ticksInUS;

  
    // Enable the task handling timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
  
    // We use Timer 5 as a one shot to trigger pending tasks
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_ONE_SHOT);
  
    // Only enable the timer interrupt when it is needed
    TimerIntDisable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
    
    // Enable the interrupt
    IntEnable(INT_WTIMER5A);


    // Setup the SysTick
    SysTickPeriodSet(SYSTICK_PERIOD);

    // Enable the interrupt
    SysTickIntEnable();

    // Start the global clock as the last thing we do
    SysTickEnable();
}


// Outputs system time in microseconds
tTime GetTimeUS(void) {
    tTime timing, systick;
    
    do {
        timing = systemTiming;
        systick = (SYSTICK_PERIOD - SysTickValueGet()) / ticksInUS;
    } while (timing != systemTiming);
    
    return timing + systick;
}

float GetTime(void) {
    return GetTimeUS() / (float)US(1);
}


// Simple SysTick handler just updates the time
void SysTickHandler(void) {
    systemTiming += sysTickTiming;
}


// Called internally to register a task
static void RegisterTask(tTask *task) {
    tTask **p;
    
    // Disable any incoming tasks temporarily
    TimerIntDisable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
    
    // Iterate through the queue until we find a 
    // later task or hit the end
    for (p = &pendingQueue; *p; p = &(*p)->next) {
        if ((*p)->target > task->target)
            break;
    }
    
    // Insert the task into the queue
    task->next = *p;
    *p = task;
}

// Setup timer to trigger an interrupt for the next task
static void SetNextTaskInt(tTime time) {
    tTime until;
  
    // Check to make sure there even is a task
    if (!pendingQueue)
        return;

    until = pendingQueue->target;
    
    // If its past the target already we should fire immediately
    if (time > until) {
        // Put a single cycle because why not
        until = 1;
    } else {
        // Calculate the next task's eta
        until = (until-time) * ticksInUS;

// Uncomment to support tasks running more than
// 36 thousand years by checking for 64bit overflow
//
//        if (((until >> 32) * ticksInUS) >> 32)
//            until = 0xffffffffffffffffULL;
//        else
//            until *= ticksInUS;
    }
    
    // Load the timer
    TimerLoadSet64(WTIMER5_BASE, until);
      
    // Enable the interrupt and timer
    // interrupt might have been disabled to prevent race conditions
    // and timer is disabled after one_shot
    TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(WTIMER5_BASE, TIMER_A);
}


// Handler used to manage waiting tasks
void WTimer5Handler(void) {
    // Get the current time with US precision
    tTime time = GetTimeUS();
    
    TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);

    // Handling any waiting tasks
    while (pendingQueue && time >= pendingQueue->target) {
        // remove it from the buffer
        tTask *task = pendingQueue;
        pendingQueue = task->next;

        task->callback(task->data);
        
        if (task->repeatTime) {
            // Reregister the function if it wants to repeat
            task->target += task->repeatTime;
            RegisterTask(task);

        } else {
            // Otherwise we stick it back in the available tasks
            task->next = unusedQueue;
            unusedQueue = task;
        }
    }
    
    // Setup the next task
    SetNextTaskInt(time);
}


// Schedules a callback function to be called in given microseconds
// The return value can be used to stop the call with CallStop
int CallInUS(tCallback callback, void *data, tTime us) {
    tTask *task;
    
    // Check if any tasks are available
    if (!unusedQueue)
        return 0;

    // Grab the next available task
    task = unusedQueue;
    unusedQueue = task->next;

    // Claim the next task id
    task->id = nextID++;
    
    // Setup the tasks info
    task->target = GetTimeUS() + us;
    // Zero repeatTime indicates it should run once
    task->repeatTime = 0;
    task->callback = callback;
    task->data = data;
      
    // Register it to be called
    RegisterTask(task);
    SetNextTaskInt(GetTimeUS());
    
    // Return the id we are using
    return task->id;
}

int CallIn(tCallback callback, void *data, float s) {
    return CallInUS(callback, data, US(s));
}


// Schedules a callback function to be called repeatedly
// The return value can be used to stop the call with CallStop
int CallEveryUS(tCallback callback, void *data, tTime us) {
    tTask *task;

    // Check if any tasks are available
    if (!unusedQueue)
        return 0;

    // Grab the next available task
    task = unusedQueue;
    unusedQueue = task->next;

    // Claim the next task id
    task->id = nextID++;
    
    // Setup the task info
    task->target = GetTimeUS() + us;
    task->repeatTime = us;
    task->callback = callback;
    task->data = data;
      
    // Register it to be called
    RegisterTask(task);
    SetNextTaskInt(GetTimeUS());
    
    // Return the id we are using
    return task->id;
}

int CallEvery(tCallback callback, void *data, float s) {
    return CallEveryUS(callback, data, US(s));
}


// Stops a pending call based on the passed identifier
void CallStop(int id) {
    tTask **p;

    // We have to just iterate all tasks to find it
    for (p = &pendingQueue; *p; p = &(*p)->next) {
        
        if ((*p)->id == id) {
            // Remove it from the queue and throw it back in the unused pile
            tTask *task = *p;
            *p = task->next;
            
            task->next = unusedQueue;
            unusedQueue = task;
        }
    }
}
    

// Busy waits for given milliseconds
static void WaitHandler(int *flag) {
    *flag = 1;
}

void WaitUS(tTime us) {
    volatile int waitFlag = 0;
    CallInUS(WaitHandler, (void*)&waitFlag, us);
    while(!waitFlag);
}
  
void Wait(float s) {
    WaitUS(US(s));
}
