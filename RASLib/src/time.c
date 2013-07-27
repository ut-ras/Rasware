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

#include "time.h"
#include "gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

// Global System Clock
static volatile tTime systemTimeUS = 0;
static long USTicks = 0;

// Relevant task info
typedef struct {
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
} tTask;

// Cyclic buffer of waiting tasks
// Must be a power of 2 for masking to work
#define TASK_BUFF_SIZE 32
#define TASK_MASK(n) ((TASK_BUFF_SIZE-1) & (n))

unsigned int taskStart = 0;
unsigned int taskEnd = 0;

tTask taskBuffer[TASK_BUFF_SIZE];

// Next identifier
int nextID = 42;


// Initializes a system timer with microsecond resolution
void InitializeSystemTime(void) {
    // Reset global clock
    systemTimeUS = 0;
  
    // Reset buffer just in case its been used
    taskStart = 0;
    taskEnd = 0;
  
    // Find the system clock divided by 1s, which results in 
    // 1 US for the timer
    USTicks = SysCtlClockGet() / US(1);
  
    // Enable the timer 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
  
    // We use the first half of the timer to keep track of the 
    // microseconds that have passed
    // The second half we use as a one shot to trigger pending tasks
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR | 
                                 TIMER_CFG_A_PERIODIC_UP | 
                                 TIMER_CFG_B_ONE_SHOT);    
  
    // Setup timer A to run for a full second
    TimerLoadSet(WTIMER5_BASE, TIMER_A, US(1) * USTicks);
    TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
  
    // Only enable the timer B interrupt when it is needed
    TimerIntDisable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
    
    // Enable the timers and interrupts
    IntEnable(INT_WTIMER5A);
    IntEnable(INT_WTIMER5B);
    TimerEnable(WTIMER5_BASE, TIMER_BOTH);
}

// Outputs system time in microseconds
tTime GetTimeUS(void) {
    return (TimerValueGet(WTIMER5_BASE, TIMER_A) / USTicks)
           + systemTimeUS;
}

float GetTime(void) {
    return ((TimerValueGet(WTIMER5_BASE, TIMER_A) / (float)USTicks)
            + systemTimeUS) / US(1);
}

// Handler that simply updates the time by one second
void WTimer5AHandler(void) {
    TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
    systemTimeUS += US(1);
}

// Called internally to register a task
static void RegisterTask(tTask *task) {
    unsigned int i, prev;
  
    // Disable any incoming tasks temporarily
    TimerIntDisable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
  
    // Start at the back and work forward moving any later tasks back  
    for (i = taskEnd; i != taskStart; i = prev) {
        prev = TASK_MASK(i-1);
      
        if (taskBuffer[prev].target > task->target) {
            taskBuffer[i] = taskBuffer[prev];
        } else {
            break;
        }
    }
    
    // Put our task into the task buffer
    taskBuffer[i] = *task;
    
    // Increase the buffer size
    taskEnd = TASK_MASK(taskEnd+1);
}

// Setup timer B to trigger an interrupt for the next task
static void SetNextTaskInt(void) {
    tTime until, time;
  
    // Check to make sure there even is a task
    if (taskEnd == taskStart) {
        TimerIntDisable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
        return;
    }

    until = taskBuffer[taskStart].target;
    time = GetTimeUS();
    
    // If its past the target already we should fire immediately
    if (time > until) {
        // Put a single cycle because why not
        until = 1;
    } else {
        // Calculate the next task's eta
        until = (until - time) * USTicks;
        
        // Check for 32bit overflow in which case we can just 
        // interrupt as late as possible. The handler will do nothing.
        if (until > 0xffffffff)
            until = 0xffffffff;
    }
    
    // Load the timer
    TimerLoadSet(WTIMER5_BASE, TIMER_B, until);
      
    // Enable the actual interrupt
    TimerIntEnable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
}

// Handler used to manage waiting tasks
void WTimer5BHandler(void) {
    // Get the current time with US precision
    tTime time = GetTimeUS();
    
    TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
  
    // If there is a task waiting, call it and 
    // remove it from the buffer
    while (taskStart != taskEnd && 
           time >= taskBuffer[taskStart].target) {
               
        tTask *task = &taskBuffer[taskStart];
      
        task->callback(task->data);
        
        // Reregister the function if it wants to repeat
        if (task->repeatTime) {
            task->target += task->repeatTime;
            RegisterTask(task);
        }
        
        // Pop the buffer
        taskStart = TASK_MASK(taskStart+1);
    }
    
    // Setup the next task
    SetNextTaskInt();
}

// Schedules a callback function to be called in given microseconds
// The return value can be used to stop the call with CallStop
int CallInUS(tCallback callback, void *data, tTime us) {
    tTask task;
  
    // Claim the next task id
    task.id = nextID++;
    
    // Setup the tasks info
    task.target = GetTimeUS() + us;
    // Zero repeatTime indicates it should run once
    task.repeatTime = 0;
    task.callback = callback;
    task.data = data;
      
    // Register it to be called
    RegisterTask(&task);
    SetNextTaskInt();
    
    // Return the id we are using
    return task.id;
}

int CallIn(tCallback callback, void *data, float s) {
    return CallInUS(callback, data, US(s));
}

// Schedules a callback function to be called repeatedly
// The return value can be used to stop the call with CallStop
int CallEveryUS(tCallback callback, void *data, tTime us) {
    tTask task;
    
    // Claim the next task id
    task.id = nextID++;
    
    // Setup the task info
    task.target = GetTimeUS() + us;
    task.repeatTime = us;
    task.callback = callback;
    task.data = data;
      
    // Register it to be called
    RegisterTask(&task);
    SetNextTaskInt();
    
    // Return the id we are using
    return task.id;
}

int CallEvery(tCallback callback, void *data, float s) {
    return CallEveryUS(callback, data, US(s));
}

// Stops a pending call based on the passed identifier
void CallStop(int id) {
    int i;
    
    // We have to just iterate all tasks to find it
    for (i = taskStart; i != taskEnd; i = TASK_MASK(i+1)) {
        
        if (taskBuffer[i].id == id) {
            // If we find it, just replace it with the Dummy function
            // It will be removed after it triggers
            taskBuffer[i].callback = Dummy;
            // Also clear the repeat time, so we don't fill up 
            // with looping dummies
            taskBuffer[i].repeatTime = 0;
            
            // IDs should be unique, so we can just return
            return;
        }
    }
}
    

// Busy waits for given milliseconds
static void WaitHandler(int *flag) {
    *flag = 1;
}

void WaitUS(tTime us) {
    volatile int waitFlag = 0;
    CallInUS(WaitHandler, (void *)&waitFlag, us);
    while(!waitFlag);
}
  
void Wait(float s) {
    WaitUS(US(s));
}
