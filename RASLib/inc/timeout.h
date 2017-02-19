//*****************************************************************************
//
// timeout - fuctions for using timeouts
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE AUTHORS OF THIS FILE
// SHALL NOT, UNDER ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of RASLib Rev0 of the RASWare package.
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

#ifndef _R_TIMEOUT_H_
#define _R_TIMEOUT_H_

#include "common.h"
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif

// Number of timeouts to internally allocate
// Surpassing this value will cleanly return an id of 0
#define TIMEOUT_COUNT 1

/**
 * Initializes a system timer with microsecond resolution
 */
void InitializeSystemTimeout(void);

/**
 * Schedules a callback function to be called after a time from the last timeout reset
 * @param callback Function that will be called
 * @param data Argument sent to the callback function when it's called
 * @param us Period between calls, specified in microseconds
 * @return ID that can be used to reset the timeout with TimeoutReset or stop the timeout with TimeoutStop
 */
int CallOnTimeoutUS(tCallback callback, void *data, tTime us);

/**
 * Schedules a callback function to be called after a time from the last timeout reset
 * @param callback Function that will be called
 * @param data Argument sent to the callback function when it's called
 * @param us Period between calls, specified in microseconds
 * @return ID that can be used to reset the timeout with TimeoutReset or stop the timeout with TimeoutStop
 */
int CallOnTimeout(tCallback callback, void *data, float s);

/**
 * reschedules the callback function to be called after the programmed time from this function call
 * @param the id of the timeout to reschedule
 */
void TimeoutReset(int timoutid);

/**
 * stops the callback function from being called
 * @param the id of the timeout to reschedule
 */
void TimeoutStop(int timeoutid);

#ifdef __cplusplus
}
#endif

#endif // _R_TIMEOUT_H_
