//*****************************************************************************
//
// time.h - real time based functions
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

#ifndef __TIME_H__
#define __TIME_H__

#include "internal.h"

// Definition of time type intended to store microseconds. 
// Will last for 584 thousand years.
typedef unsigned long long tTime;

// Macro for converting seconds to microseconds
// Also handles conversion from floats
#define US(s) ((tTime)((s)*1000*1000))

// Initializes a system timer with millisecond resolution
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

#endif // __TIME_H__
