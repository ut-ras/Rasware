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

#ifndef _R_TIME_H_
#define _R_TIME_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


// Definition of time type intended to store microseconds. 
// Will last for 584 thousand years.
typedef unsigned long long tTime;

// Macro for converting seconds to microseconds
// Also handles conversion from floats
#define US(s) ((tTime)((s)*1000*1000))

// Number of ticks between SysTick interrupts
// Limited to 24 bits on the LM4F
#define SYSTICK_PERIOD 0xffffff

// Number of tasks to internally allocate
// Surpassing this value will cleanly return an id of 0
#define TASK_COUNT 128


// Initializes a system timer with microsecond resolution
void InitializeSystemTime(void);

// Outputs system time
tTime GetTimeUS(void);
float GetTime(void);

// Schedules a callback function to be called in given time
// The return value can be used to stop the call with CallStop
int CallInUS(tCallback callback, void *data, tTime us);
int CallIn(tCallback callback, void *data, float s);

// Schedules a callback function to be called repeatedly
// The return value can be used to stop the call with CallStop
int CallEveryUS(tCallback callback, void *data, tTime us);
int CallEvery(tCallback callback, void *data, float s);

// Stops a pending call based on the passed identifier
void CallStop(int id);

// Busy waits for given time
void WaitUS(tTime ms);
void Wait(float s);

#ifdef __cplusplus
}
#endif

#endif // _R_TIME_H_
