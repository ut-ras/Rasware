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

// Definition of time type intended to store microseconds. 
// Will last for 584 thousand years.
typedef unsigned long long time_t;

// Initializes a system timer with millisecond resolution
void InitializeSystemTime(void);

// Outputs system time
time_t GetTimeUS(void);
time_t GetTimeS(void);

// Schedules a callback function to be called in given time
void CallInUS(void (*callback)(void*), void *data, time_t us);
void CallInS(void (*callback)(void*), void *data, time_t s);

// Schedules a callback function to be called repeatedly
void CallEveryUS(void (*callback)(void*), void *data, time_t us);
void CallEveryS(void (*callback)(void*), void *data, time_t s);

// Busy waits for given time
void WaitUS(time_t ms);
void WaitS(time_t s);

#endif // __TIME_H__
