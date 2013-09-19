//*****************************************************************************
//
// sonar - Software Sonar driver
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

#ifndef _R_SONAR_H_
#define _R_SONAR_H_

#include "gpio.h"
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif


// Constants used for sonar timing
// Each is given in units of microseconds
#define SONAR_TIMEOUT 36000
#define SONAR_MAX     18000
#define SONAR_DELAY   10000
#define SONAR_PULSE   10

// Definition of struct Sonar in sonar.c
typedef struct Sonar tSonar;

/**
 * Initializes a sonar on a pair of pin
 * @param trigger Pin plugged into the sonar's tigger line
 * @param echo Pin plugged into the sonar's echo line
 * @return Pointer to an initialized tSonar, can be used by the SonarRead functions
 */
tSonar *InitializeSonar(tPin trigger, tPin echo);

/**
 * Returns the distance measured from the sonar
 * @param snr Pointer to an initialized tSonar, returned by InitializeSonar
 * @return The distance measured as a percentage of maximum range of the sonar. If no response is detected, a value of infinity is returned. 
 * Note: if the sonar is not continously reading, then the function will busy wait for the results
 */
float SonarRead(tSonar *snr);

/**
 * Sets up an sonar to be run in the background
 * @param snr Pointer to an initialized tSonar, returned by InitializeSonar
 * @param callback Function called the next time the sonar read completes, in which a call to SonarRead will return with the newly obtained value immediately
 * @param data Argument sent to the provided callback function whenever it is called
 */
void SonarBackgroundRead(tSonar *snr, tCallback callback, void *data);

/**
 * Sets up an sonar to read indefinitly
 * @param snr Pointer to an initialized tSonar, returned by InitializeSonar
 * @param us Time between calls to read the sonar in micro seconds
 * Note: Any following calls to SonarRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the sonar read to complete, the sonar will fire as fast as possible without overlap
 */
void SonarReadContinuouslyUS(tSonar *snr, tTime us);

/**
 * Sets up an sonar to read indefinitly
 * @param snr Pointer to an initialized tSonar, returned by InitializeSonar
 * @param s Time between calls to read the sonar in seconds
 * Note: Any following calls to SonarRead will return the most recent value
 * Note: If the passed time between calls is less than the time it takes for the sonar read to complete, the sonar will fire as fast as possible without overlap
 */
void SonarReadContinuously(tSonar *snr, float s);


#ifdef __cplusplus
}
#endif

#endif // _R_SONAR_H_
