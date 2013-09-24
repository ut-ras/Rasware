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

#include "sonar.h"

#include <math.h>


// Definition of struct Sonar
// Defined to tSonar in sonar.h
struct Sonar {
    // The last read sonar value
    float value;
    // The time when the return pulse started
    tTime start;
    
    // Callback information
    tCallback callback;
    void *data;
    
    // Which pins the sonar is plugged into
    tPin trigger;
    tPin echo;
    
    // The id for the timeout interrupt
    int timeout;
    
    // State machine for the sonar
    volatile enum {
        READY,
        PULSE,
        WAIT,
        TIMING,
        CALLBACK,
        DELAY,
        PENDING
    } state : 4;
    
    // If it is in continous mode
    tBoolean continous : 1;
};

// buffer for sonar state structs
// There can only be the total count of pins/2 since each
// sonar needs 2 pins
tSonar sonarBuffer[PIN_COUNT/2];

int sonarCount = 0;


// Function to initialize an encoder on a pair of pins
// The returned pointer can be used by the SonarRead functions
tSonar *InitializeSonar(tPin trigger, tPin echo) {
    // Grab the next sonar
    tSonar *snr = &sonarBuffer[sonarCount++];
    
    // Setup initial data
    snr->value = 0;
    snr->state = READY;
    snr->continous = false;
    
    // Assign the pins
    snr->trigger = trigger;
    snr->echo = echo;
    
    // Return the new sonar
    return snr;
}

// Declared here to avoid cyclic dependency below
static void BeginSonarSequence(tSonar *snr);

// Handler to the delay after full reads
static void DelayHandler(tSonar *snr) {
    // Check if there is pending operation
    if (snr->state == PENDING)
        BeginSonarSequence(snr);
    else
        snr->state = READY;
}

// Function to start delaying after a full read
// to preven false responses
static void SonarDelay(tSonar *snr) {
    // If the state was changed previously
    // we leave it
    if (snr->state == CALLBACK)
        snr->state = DELAY;
    
    // Call the DelayHandler later
    CallInUS(DelayHandler, snr, SONAR_DELAY);
}

// Handler to the echo pin from the sonar
static void EchoHandler(tSonar *snr) {
    // Check if we started measuring
    if (GetPin(snr->echo)) {
        // If so start timing
        snr->start = GetTimeUS();
        snr->state = TIMING;
        
    } else {
        // Stop the timeout first
        CallStop(snr->timeout);
        
        // Calculate the measured value
        snr->value = (GetTimeUS() - snr->start) / (float)SONAR_MAX;
        
        // Call the callback with the acquired data
        snr->state = CALLBACK;
        snr->callback(snr->data);
        
        // We've finished the read, but we still need to delay
        SonarDelay(snr);
    }  
}

// Handler for when the sonar gets no response
static void TimeoutHandler(tSonar *snr) {
    // Stop the echo pin function
    CallOnPin(0, 0, snr->echo);
    
    // Store infinity as a bad value
    snr->value = INFINITY;
    
    // Call the callback
    snr->state = CALLBACK;
    snr->callback(snr->data);
    
    // We've finished the read, but we still need to delay 
    SonarDelay(snr);
}

// Handler for after the pulse has fired for long enough
static void PulseHandler(tSonar *snr) {
    // Lower the pin
    snr->state = WAIT;
    SetPin(snr->trigger, false);
    
    // Wait for either the echo line to be raised
    // or the timeout to occur
    CallOnPin(EchoHandler, snr, snr->echo);
    snr->timeout = CallInUS(TimeoutHandler, snr, SONAR_TIMEOUT);
}

// Function for starting the sonar sequence
static void BeginSonarSequence(tSonar *snr) {
    // Raise the trigger pin to start the sonar
    snr->state = PULSE;
    SetPin(snr->trigger, true);

    // Lower the pin later
    CallInUS(PulseHandler, snr, SONAR_PULSE);
}

// This function sets up a sonar to be read in the background. 
// A callback can be passed, in which a call to SonarRead 
// will return with the newly obtained value immediately
void SonarBackgroundRead(tSonar *snr, tCallback callback, void *data) {
    // Setup the callback
    snr->callback = callback ? callback : Dummy;
    snr->data = data;
    
    // Begin the sonar sequence if we can
    if (snr->state == READY)
        BeginSonarSequence(snr);
    else if (snr->state == DELAY || snr->state == CALLBACK)
        snr->state = PENDING;
}

// This function returns the distance measured as a percentage of 
// maximum range of the sonar. If no response is detected, a value of 
// infinity is returned. If the sonar is not continously reading,
// then the function will busy wait for the results
float SonarRead(tSonar *snr) {
    // Check if we need to read a value
    if (snr->state != CALLBACK && !snr->continous) {
        // Just call background read and then busy wait
        SonarBackgroundRead(snr, 0, 0);
        while(snr->state != DELAY && snr->state != READY);
    }
        
    // Return the most recent value
    return snr->value;
}

// These function set up a sonar to read indefinitly
// Any following calls to SonarRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the sonar to complete, the sonar will fire as fast as possible without overlap

// Handler for reading a single value
static void SingleReadHandler(tSonar *snr) {
    SonarBackgroundRead(snr, 0, 0);
}

// Handler for reading as fast as possible
static void ContinuousReadHandler(tSonar *snr) {
    SonarBackgroundRead(snr, ContinuousReadHandler, snr);
}

void SonarReadContinuouslyUS(tSonar *snr, tTime us) {
    // Set the continous flag
    snr->continous = true;
    
    // Check if there isn't enough time for the sensor to be read
    if (us <= SONAR_TIMEOUT + SONAR_PULSE)
        // If there isn't, read as fast as possible
        SonarBackgroundRead(snr, ContinuousReadHandler, snr);
    else
        // Otherwise just periodically call the single read handler
        CallEveryUS(SingleReadHandler, snr, us);
}

void SonarReadContinuously(tSonar *snr, float s) {
    SonarReadContinuouslyUS(snr, US(s));
}
