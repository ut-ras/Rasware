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

// Global System Clock
static volatile time_t systemTimeUS = 0;
static long USTicks = 0;

#define US_IN_S (1000*1000)

// Relevant task info
struct task {
		time_t target;
	  time_t repeatTime;
		void *data;
		void (*callback)(void*);
};

// Cyclic buffer of waiting tasks
#define TASK_BUFF_SIZE 32
#define TASK_MASK(n) ((TASK_BUFF_SIZE-1) & (n))

unsigned int taskStart = 0;
unsigned int taskEnd = 0;

struct task taskBuffer[TASK_BUFF_SIZE];


// Initializes a system timer with microsecond resolution
void InitializeSystemTime(void) {
    // Reset global clock
		systemTimeUS = 0;
	
		// Reset buffer just in case its been used
		taskStart = 0;
		taskEnd = 0;
	
		// Find the system clock divided by 1000000, which results in 
		// 1 US for the timer
		USTicks = SysCtlClockGet() / US_IN_S;
	
		// Enable the timer 
		SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
	
		// We use the first half of the timer to keep track of the 
		// microseconds that have passed
		// The second half we use as a one shot to trigger pending tasks
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR | 
                                 TIMER_CFG_A_PERIODIC_UP | 
                                 TIMER_CFG_B_ONE_SHOT);		
	
		// Setup timer A to run for a full second
		TimerLoadSet(WTIMER5_BASE, TIMER_A, US_IN_S * USTicks);
		TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
	
		// Only enable the timer B interrupt when it is needed
		TimerIntDisable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
		
    // Enable the timers and interrupts
		IntEnable(INT_WTIMER5A);
		IntEnable(INT_WTIMER5B);
		TimerEnable(WTIMER5_BASE, TIMER_BOTH);
}

// Outputs system time in microseconds
time_t GetTimeUS(void) {
		return systemTimeUS + 
					 (TimerValueGet(WTIMER5_BASE, TIMER_A) / USTicks);
}

time_t GetTimeS(void) {
		return GetTimeUS() / US_IN_S;
}

// Handler that simply updates the time by one second
void WTimer5AHandler(void) {
		TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
		systemTimeUS += US_IN_S;
}

// Called internally to register a task
static void RegisterTask(struct task *task) {
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
		time_t usUntil;
	
		// Check to make sure there even is a task
		if (taskEnd == taskStart) {
				TimerIntDisable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
				return;
		}

		// Calculate the next task's eta
		usUntil = taskBuffer[taskStart].target - GetTimeUS();
			
		// Check for 32bit overflow in which case we can just 
		// interrupt as late as possible. The handler will do nothing.
		TimerLoadSet(WTIMER5_BASE, TIMER_B, 
								 (usUntil > 0xffffffff) ? 0xffffffff : usUntil);
			
		// Enable the actual interrupt
		TimerIntEnable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
}

// Handler used to manage waiting tasks
void WTimer5BHandler(void){
		TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
	
		// If there is a task waiting, call it and 
		// remove it from the buffer
		while (taskStart != taskEnd && 
					 systemTimeUS >= taskBuffer[taskStart].target) {
				struct task *task = &taskBuffer[taskStart];
			
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
void CallInUS(void (*callback)(void*), void *data, time_t us) {
		struct task task;
	
		task.target = systemTimeUS + us;
		// Zero repeatTime indicates it should run once
		task.repeatTime = 0;
		task.callback = callback;
		task.data = data;
			
		// Register it to be called
		RegisterTask(&task);
		SetNextTaskInt();
}

void CallInS(void (*callback)(void*), void *data, time_t s) {
		CallInUS(callback, data, s*US_IN_S);
}

// Schedules a callback function to be called repeatedly
void CallEveryUS(void (*callback)(void*), void *data, time_t us) {
		struct task task;
	
		task.target = systemTimeUS + us;
		task.repeatTime = us;
		task.callback = callback;
		task.data = data;
			
		// Register it to be called
		RegisterTask(&task);
		SetNextTaskInt();
}

void CallEveryS(void (*callback)(void*), void *data, time_t s) {
		CallEveryUS(callback, data, s*US_IN_S);
}

// Busy waits for given milliseconds
static void WaitHandler(void *flag) {
		*(int*)flag = 1;
}

void WaitUS(time_t us) {
		int waitFlag = 0;
		CallInUS(WaitHandler, &waitFlag, us);
		while(!waitFlag);
}

void WaitS(time_t s) {
		WaitUS(s*US_IN_S);
}
		
