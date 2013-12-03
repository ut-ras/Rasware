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

#include "time.h"
#include "i2c.h"


#define ADS7830 0x48


// Definition of struct LineSensor
// Defined to tLineSensor in i2c.h
typedef struct I2CLineSensor {
    // LineSensor 'abstract' functions
    unsigned char (*Read)(struct I2CLineSensor *ls, float threshold);
    tBoolean (*ReadArray)(struct I2CLineSensor *ls, float *array);
    void (*BackgroundRead)(struct I2CLineSensor *ls, tCallback callback, void *data);
    void (*ReadContinuouslyUS)(struct I2CLineSensor *ls, tTime us);
    void (*ReadContinuously)(struct I2CLineSensor *ls, float s);
    
    // Internally used I2C module
    tI2C *i2c;
    
    // Full i2c address 
    unsigned int address;
    
    // Array of values and index in current read
    unsigned char values[8];
    unsigned char index;
    unsigned char command;
    
    // Callback data
    tCallback callback;
    void *data;
    
    // Some state variables
    tBoolean in_callback : 1;
    tBoolean continous : 1;
    volatile tBoolean pending : 1;
} tI2CLineSensor;


// Buffer of line sensor structs to use
// There can only be up to the number of I2C modules times 4
// since there can be 4 addresses on each bus
static tI2CLineSensor lineSensorBuffer[I2C_COUNT * 4];

static int lineSensorCount = 0;

// Internally used handler to trigger next
// sensor read in the line sensor
static void I2CLineSensorHandler(tI2CLineSensor *ls) {
    if (ls->index >= 8) {
        // If we're finished we call the callback
        ls->in_callback = true;
        ls->callback(ls->data);
        ls->in_callback = false;
        ls->pending = false;
        
    } else {
        // Otherwise we move on to the next sensor
        unsigned char index = ls->index++;
        
        // Setup the i2c command
        ls->command = 0x84 | (index << 4);
        
        // Make the actual request
        I2CBackgroundRequest(ls->i2c, ls->address,
                             &ls->command, 1,
                             &ls->values[index], 1,
                             I2CLineSensorHandler, ls);
    }
}

// This function sets up a LineSensor to be run in the background
// A callback can be passed, in which a call to LineSensorRead 
// will return with the newly obtained value immediately
static void I2CLineSensorBackgroundRead(tI2CLineSensor *ls, tCallback callback, void *data) {
    // Store the callback information
    ls->callback = callback ? callback : Dummy;
    ls->data = data;
    
    // Reset the index and flags
    ls->index = 0;
    ls->pending = true;
    
    // Call the handler itself to start reading the values
    I2CLineSensorHandler(ls);
}


// This function returns the values measured as a bit-packed byte
// each bit is matched against a threshold that is passed.
// If the LineSensor is not continously reading,
// then the function will busy wait for the results
static unsigned char I2CLineSensorRead(tI2CLineSensor *ls, float threshold) {
    unsigned char thresh;
    unsigned char output = 0x0;
    int i;
    
    // Check if we need to read a value
    if (!ls->in_callback && !ls->continous) {
        // Just call LineSensorBackgroundRead and busy wait
        I2CLineSensorBackgroundRead(ls, 0, 0);
        while (ls->pending);
    }
    
    // Check for any errors
    if (!I2CSuccess(ls->i2c))
        return 0xff;
    
    // Stick to unsigned chars for efficiency
    thresh = (unsigned char)(0xff * threshold);
    
    // Calculate the byte and return it
    for (i=0; i < 8; i++) {
        if (ls->values[i] > thresh)
            output |= (1 << i);
    }
    
    return output;
}


// This function returns the values measured as an
// array of ratios placed in the passed memory location.
// If the LineSensor is not continously reading,
// then the function will busy wait for the results
static tBoolean I2CLineSensorReadArray(tI2CLineSensor *ls, float *array) {
    int i;
    
    // Check if we need to read a value
    if (!ls->in_callback && !ls->continous) {
        // Just call LineSensorBackgroundRead and busy wait
        I2CLineSensorBackgroundRead(ls, 0, 0);
        while (ls->pending);
    }
    
    // Check for any errors
    if (!I2CSuccess(ls->i2c)) {
        for (i=0; i < 8; i++)
            array[i] = INFINITY;
        
        return false;
    }
    
    // Calculate the values
    for (i=0; i < 8; i++) {
        array[i] = (ls->values[i] / (float)(0x100));
    }
    
    return true;
}


// Handler for reading a single set of values
static void SingleReadHandler(tI2CLineSensor *ls) {
    I2CLineSensorBackgroundRead(ls, 0, 0);
}

// Handler for reading as fast as possible
static void ContinuousReadHandler(tI2CLineSensor *ls) {
    I2CLineSensorBackgroundRead(ls, ContinuousReadHandler, ls);
}

// These function set up a LineSensor to read indefinitly
// Any following calls to LineSensorRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the LineSensor to complete, the LineSensor will read as fast as possible without overlap
static void I2CLineSensorReadContinuouslyUS(tI2CLineSensor *ls, tTime us) {
    // Set the continous flag
    ls->continous = true;
    
    // Check if there isn't enough time for the sensor to be read
    // NOTE: This is unknown at the time and may be calculatable
    // For now only a time of 0 will switch to the fast as possible solution
    if (us <= 1)
        // If there isn't, read as fast as possible
        I2CLineSensorBackgroundRead(ls, ContinuousReadHandler, ls);
    else
        // Otherwise just periodically call the single read handler
        CallEveryUS(SingleReadHandler, ls, us);
}

static void I2CLineSensorReadContinuously(tI2CLineSensor *ls, float s) {
    I2CLineSensorReadContinuouslyUS(ls, US(s));
}


// Function to initialize a line sensor on a pair of pins
// The returned pointer can be used by the LineSensorRead functions
tI2CLineSensor *_InitializeI2CLineSensor(tI2C *i2c, unsigned int address) {
    // Grab the next line sensor
    tI2CLineSensor *ls = &lineSensorBuffer[lineSensorCount++];
    
    // Keep track of the I2C module
    ls->i2c = i2c;
    
    // Create the actual address
    ls->address = ADS7830 | (0x3 & address);
    
    // Set parent method/functions 
    ls->Read = I2CLineSensorRead;
    ls->ReadArray = I2CLineSensorReadArray;
    ls->BackgroundRead = I2CLineSensorBackgroundRead;
    ls->ReadContinuouslyUS = I2CLineSensorReadContinuouslyUS;
    ls->ReadContinuously = I2CLineSensorReadContinuously;
    
    // Return the initialized line sensor
    return ls;
}
