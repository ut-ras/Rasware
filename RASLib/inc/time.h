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


/**
 * Initializes a system timer with microsecond resolution
 */
void InitializeSystemTime(void);

/**
 * Outputs system time
 * @return System time in microseconds
 */
 tTime GetTimeUS(void);

/**
 * Outputs system time
 * @return System time in seconds
 */
float GetTime(void);

/**
 * Schedules a callback function to be called in given time
 * @param callback Function that will be called
 * @param data Argument sent to the callback function when it's called
 * @param us Period between calls, specified in microseconds
 * @return ID that can be used to stop the call with CallStop
 */
int CallInUS(tCallback callback, void *data, tTime us);

/**
 * Schedules a callback function to be called in given time
 * @param callback Function that will be called
 * @param data Argument sent to the callback function when it's called
 * @param s Period between calls, specified in seconds
 * @return ID that can be used to stop the call with CallStop
 */
int CallIn(tCallback callback, void *data, float s);

/**
 * Schedules a callback function to be called repeatedly
 * @param callback Function that is called repeatedly
 * @param data Argument sent to the callback function each time it's called
 * @param us Period between calls, specified in microseconds
 * @return ID that can be used to stop the call with CallStop
 */
int CallEveryUS(tCallback callback, void *data, tTime us);

/**
 * Schedules a callback function to be called repeatedly
 * @param callback Function that is called repeatedly
 * @param data Argument sent to the callback function each time it's called
 * @param s Period between calls, specified in seconds
 * @return ID that can be used to stop the call with CallStop
 */
int CallEvery(tCallback callback, void *data, float s);

/**
 * Stops a pending call based on the passed identifier
 * @param id Value returned by CallEvery and CallIn functions
 */
void CallStop(int id);

/**
 * Busy waits for given time
 * @param ms Amount of time to wait, specified in milliseconds
 */
void WaitUS(tTime ms);

/**
 * Busy waits for given time
 * @param s Amount of time to wait, specified in seconds
 */
void Wait(float s);

#ifdef __cplusplus
}
#endif

#endif // _R_TIME_H_
