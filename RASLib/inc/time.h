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


// Definition of time type intended to store milliseconds. 
// Will last for 584 million years.
typedef unsigned long long time_t;

// Initializes a system timer with millisecond resolution
void InitializeSystemTime(void);

// Outputs system time in milliseconds
time_t GetTime(void);

// Schedules a callback function to be called in given milliseconds
void CallIn(void (*callback)(void*), void *data, time_t ms);

// Schedules a callback function to be called repeatedly
void CallEvery(void (*callback)(void*), void *data, time_t ms);

// Busy waits for given milliseconds
void Wait(time_t ms);
