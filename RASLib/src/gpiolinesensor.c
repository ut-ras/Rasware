//*****************************************************************************
//
// linesensor - I2C based line sensor
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

#include <math.h>

#include "gpio.h"
#include "time.h"

// Constants used for line sensor timing
// Each is given in units of microseconds
#define GPIO_LINE_SENSOR_TIMEOUT 2500
#define GPIO_LINE_SENSOR_MAX     2500
#define GPIO_LINE_SENSOR_PULSE   10
#define GPIO_LINE_SENSOR_INF     0xffffffffffffffff

// Internally used Pin specific sensor information
typedef struct GPIOPinSensor {
    // Pin in use
    tPin pin;
    unsigned char mask;
    
    // Timing information
    tTime timing;
    
    // Pointer to containing line sensor
    struct GPIOLineSensor *ls;
} tGPIOPinSensor;


// Definition of struct GPIOLineSensor
// Defined to tGPIOLineSensor in gpiolinesensor.h
typedef struct GPIOLineSensor {
    // LineSensor 'abstract' functions
    unsigned char (*Read)(struct GPIOLineSensor *ls, float threshold);
    tBoolean (*ReadArray)(struct GPIOLineSensor *ls, float *array);
    void (*BackgroundRead)(struct GPIOLineSensor *ls, tCallback callback, void *data);
    void (*ReadContinuouslyUS)(struct GPIOLineSensor *ls, tTime us);
    void (*ReadContinuously)(struct GPIOLineSensor *ls, float s);
    
    // General timing information
    tTime start_time;
    
    // Bit-field of pending pins
    volatile unsigned char pending;
    
    // Pin specific timing information
    tGPIOPinSensor pins[8];
    
    // Stored results used when read continuously
    float values[8];
    
    // Callback data
    tCallback callback;
    void *data;
    
    // The id for the timeout interrupt
    int timeout;
    
    // Some state variables
    tBoolean in_callback : 1;
    tBoolean continous : 1;
} tGPIOLineSensor;


// Buffer of line sensor structs to use
// There can only be up to PIN_COUNT / 8
// since each sensor uses 8 pins
static tGPIOLineSensor lineSensorBuffer[PIN_COUNT / 8];

static int lineSensorCount = 0;


// Internally used handler measure discharge time
static void PinHandler(tGPIOPinSensor *ps) {
    // Get the masked value
    unsigned char res = ps->ls->pending ^ ps->mask;
    
    // Get timing and update count
    ps->timing = GetTimeUS();
    ps->ls->pending &= ~ps->mask;

    // Check if all the pins have finished
    if (res == 0) {
        // Stop the timeout
        CallStop(ps->ls->timeout);
        
        // And call the callback
        ps->ls->in_callback = true;
        ps->ls->callback(ps->ls->data);
        ps->ls->in_callback = false;
    }
}


// Handler for when the sensor times out
static void TimeoutHandler(tGPIOLineSensor *ls) {
    // Clear the pending pins
    ls->pending = 0;
    
    // And call the callback
    ls->in_callback = true;
    ls->callback(ls->data);
    ls->in_callback = false;
}


// Handler for after the pulse has fired for long enough
static void PulseHandler(tGPIOLineSensor *ls) {
    int i;
    
    // Store the current time
    ls->start_time = GetTimeUS();
    
    // Set each pin to input and start timing
    for (i = 0; i < 8; i++) {
        GetPin(ls->pins[i].pin);
    }
    
    // Also start the timeout
    ls->timeout = CallInUS(TimeoutHandler, ls, GPIO_LINE_SENSOR_TIMEOUT);
}




// This function sets up a GPIOLineSensor to be run in the background
// A callback can be passed, in which a call to GPIOLineSensorRead 
// will return with the newly obtained value immediately
static void GPIOLineSensorBackgroundRead(tGPIOLineSensor *ls, tCallback callback, void *data) {
    int i;
    
    // Store the callback information
    ls->callback = callback ? callback : Dummy;
    ls->data = data;
    
    // Reset the pending mask
    ls->pending = 0xff;
    
    // Raise the pins and clear the timing
    for (i = 0; i < 8; i++) {
        ls->pins[i].timing = GPIO_LINE_SENSOR_INF;
        SetPin(ls->pins[i].pin, true);
    }
    
    // Start the pulse handler
    CallInUS(PulseHandler, ls, GPIO_LINE_SENSOR_PULSE);
}


// This function returns the values measured as a bit-packed byte
// each bit is matched against a threshold that is passed.
// If the LineSensor is not continously reading,
// then the function will busy wait for the results
static unsigned char GPIOLineSensorRead(tGPIOLineSensor *ls, float threshold) {
    tTime thresh;
    unsigned char output = 0x0;
    int i;
    
    // Check if we need to read a value
    if (!ls->in_callback && !ls->continous) {
        // Just call GPIOLineSensorBackgroundRead and busy wait
        GPIOLineSensorBackgroundRead(ls, 0, 0);
        while (ls->pending);
    }
    
    // Special case for continuous readings
    if (ls->continous) {
        for (i=0; i < 8; i++) {
            if (ls->values[i] > threshold)
                output |= (1 << i);
        }
        
        return output;
    }
    
    // Stick to tTime for efficiency
    thresh = (tTime)(GPIO_LINE_SENSOR_MAX * threshold);
    
    // Calculate the byte and return it
    for (i=0; i < 8; i++) {
        tTime timing = ls->pins[i].timing;
        
        if ((timing - ls->start_time) > thresh)
            output |= (1 << i);
    }
    
    return output;
}


// This function returns the values measured as an
// array of ratios placed in the passed memory location.
// If the GPIOLineSensor is not continously reading,
// then the function will busy wait for the results
static tBoolean GPIOLineSensorReadArray(tGPIOLineSensor *ls, float *array) {
    int i;
    
    // Check if we need to read a value
    if (!ls->in_callback && !ls->continous) {
        // Just call LineSensorBackgroundRead and busy wait
        GPIOLineSensorBackgroundRead(ls, 0, 0);
        while (ls->pending);
    }
    
    // Special case for continuous readings
    if (ls->continous) {
        for (i=0; i < 8; i++) {
            array[i] = ls->values[i];
        }
        
        return true;
    }
    
    // Calculate the values
    for (i=0; i < 8; i++) {
        tTime timing = ls->pins[i].timing;
        
        if (timing == GPIO_LINE_SENSOR_INF)
            array[i] = INFINITY;
        else
            array[i] = ((timing - ls->start_time) / (float)(GPIO_LINE_SENSOR_MAX));
    }
    
    return true;
}


// Helper function for continuous reads
static void CalculateSingleRead(tGPIOLineSensor *ls) {
    int i;
    
    // Calculate the values
    for (i=0; i < 8; i++) {
        tTime timing = ls->pins[i].timing;
        
        if (timing == GPIO_LINE_SENSOR_INF)
            ls->values[i] = INFINITY;
        else
            ls->values[i] = ((timing - ls->start_time) / (float)(GPIO_LINE_SENSOR_MAX));
    }
}

// Handler for reading a single set of values
static void SingleReadHandler(tGPIOLineSensor *ls) {
    GPIOLineSensorBackgroundRead(ls, CalculateSingleRead, ls);
}

// Handler for reading as fast as possible
static void ContinuousReadHandler(tGPIOLineSensor *ls) {
    CalculateSingleRead(ls);
    GPIOLineSensorBackgroundRead(ls, ContinuousReadHandler, ls);
}

// These function set up a GPIOLineSensor to read indefinitly
// Any following calls to GPIOLineSensorRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the GPIOLineSensor to complete, the GPIOLineSensor will read as fast as possible without overlap
static void GPIOLineSensorReadContinuouslyUS(tGPIOLineSensor *ls, tTime us) {
    // Set the continous flag
    ls->continous = true;
    
    // Check if there isn't enough time for the sensor to be read
    if (us <= GPIO_LINE_SENSOR_MAX + GPIO_LINE_SENSOR_PULSE)
        // If there isn't, read as fast as possible
        GPIOLineSensorBackgroundRead(ls, ContinuousReadHandler, ls);
    else
        // Otherwise just periodically call the single read handler
        CallEveryUS(SingleReadHandler, ls, us);
}

static void GPIOLineSensorReadContinuously(tGPIOLineSensor *ls, float s) {
    GPIOLineSensorReadContinuouslyUS(ls, US(s));
}

// Function to initialize a line sensor on the provided pins
// The returned pointer can be used by the GPIOLineSensorRead functions
tGPIOLineSensor *_InitializeGPIOLineSensor(tPin p0, tPin p1, tPin p2, tPin p3, tPin p4, tPin p5, tPin p6, tPin p7) {
    int i;
    
    // Grab the next line sensor
    tGPIOLineSensor *ls = &lineSensorBuffer[lineSensorCount++];
    
    // Keep track of the pins
    ls->pins[0].pin = p0;
    ls->pins[1].pin = p1;
    ls->pins[2].pin = p2;
    ls->pins[3].pin = p3;
    ls->pins[4].pin = p4;
    ls->pins[5].pin = p5;
    ls->pins[6].pin = p6;
    ls->pins[7].pin = p7;

    // Setup the pin handlers
    for (i = 0; i < 8; i++) {
        ls->pins[i].mask = 1 << i;
        ls->pins[i].ls = ls;
        
        CallOnPinFalling(PinHandler, &ls->pins[i], ls->pins[i].pin);
    }
    
    // Set parent method/functions 
    ls->Read = GPIOLineSensorRead;
    ls->ReadArray = GPIOLineSensorReadArray;
    ls->BackgroundRead = GPIOLineSensorBackgroundRead;
    ls->ReadContinuouslyUS = GPIOLineSensorReadContinuouslyUS;
    ls->ReadContinuously = GPIOLineSensorReadContinuously;
    
    // Return the initialized line sensor
    return ls;
}
