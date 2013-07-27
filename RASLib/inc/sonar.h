//*****************************************************************************
//
// sonar.h - software sonar driver
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

#ifndef __SONAR_H__
#define __SONAR_H__

#include "gpio.h"
#include "time.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

// Constants used for sonar timing
// Each is given in units of microseconds
#define SONAR_TIMEOUT 36000
#define SONAR_MAX     18000
#define SONAR_DELAY   10000
#define SONAR_PULSE   10

// Definition of struct Sonar in sonar.c
typedef struct Sonar tSonar;

// Function to initialize an encoder on a pair of pins
// The returned pointer can be used by the SonarRead functions
tSonar *InitializeSonar(tPin trigger, tPin echo);

// This function returns the distance measured as a percentage of 
// maximum range of the sonar. If no response is detected, a value of 
// infinity is returned. If the sonar is not continously reading,
// then the function will busy wait for the results
float SonarRead(tSonar *snr);

// This function sets up a sonar to be read in the background. 
// A callback can be passed, in which a call to SonarRead 
// will return with the newly obtained value immediately
void SonarBackgroundRead(tSonar *snr, tCallback callback, void *data);

// These function set up a sonar to read indefinitly
// Any following calls to SonarRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the sonar to complete, the sonar will fire as fast as possible without overlap
void SonarReadContinouslyUS(tSonar *snr, tTime us);
void SonarReadContinously(tSonar *snr, float s);

#endif //  __SONAR_H__
