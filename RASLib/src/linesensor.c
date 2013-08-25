#include "linesensor.h"
#include "i2c.h"
#include "UART.h"
#include <stdlib.h>
#include <stdarg.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/i2c.h>
#include "i2c.h"
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <inc/hw_i2c.h>
#include <math.h>

#define ADS7830 0x48

static const unsigned char REQUEST_CMD = 0x84;

// Definition of struct LineSensor
// Defined to tLineSensor in i2c.h
struct LineSensor {
    // Internally used I2C module
    tI2C *i2c;
    
    // Array of values and index in current read
    unsigned char values[8];
    int index;
    
    // Callback data
    tCallback callback;
    void *data;
    
    // Some state variables
    tBoolean in_callback : 1;
    tBoolean continous : 1;
    volatile tBoolean pending : 1;
};

// Buffer of line sensor structs to use
// There can only be the total count of pins/2 since each
// line sensor needs 2 pins
tLineSensor lineSensorBuffer[PIN_COUNT / 2];

int lineSensorCount = 0;
        

// Function to initialize a line sensor on a pair of pins
// The returned pointer can be used by the LineSensorRead functions
tLineSensor *InitializeLineSensor(tPin sda, tPin scl) {
    // Grab the next line sensor
    tLineSensor *ls = &lineSensorBuffer[lineSensorCount++];
    
    // Initialize the I2C module
    ls->i2c = InitializeI2C(sda, scl);
    
    // Return the initialized line sensor
    return ls;
}

// Internally used handler to trigger next
// sensor read in the line sensor
void LineSensorHandler(tLineSensor *ls) {
    if (ls->index == 8) {
        // If we're finished we call the callback
        ls->in_callback = true;
        ls->callback(ls->data);
        
        // release all the flags
        ls->in_callback = false;
        ls->pending = false;
        
    } else {
        // Otherwise we move on to the next sensor
        ls->index++;
        
        I2CBackgroundRequest(ls->i2c, ADS7830,
                             &REQUEST_CMD, 1,
                             &ls->values[ls->index], 1,
                             LineSensorHandler, ls);
    }
}     


// This function sets up a LineSensor to be run in the background
// A callback can be passed, in which a call to LineSensorRead 
// will return with the newly obtained value immediately
void LineSensorBackgroundRead(tLineSensor *ls, tCallback callback, void *data) {
    // Store the callback information
    ls->callback = callback ? callback : Dummy;
    ls->data = data;
    
    // Reset the index;
    ls->index = 0;
    
    // Call the handler itself to start reading the values
    LineSensorHandler(ls);
}


// This function returns the values measured as a bit-packed byte
// each bit is matched against a threshold that is passed.
// If the LineSensor is not continously reading,
// then the function will busy wait for the results
unsigned char LineSensorRead(tLineSensor *ls, float threshold) {
    unsigned char thresh;
    unsigned char output = 0x0;
    int i;
    
    // Check if we need to read a value
    if (!ls->in_callback && !ls->continous) {
        // Just call LineSensorBackgroundRead and busy wait
        LineSensorBackgroundRead(ls, 0, 0);
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
void LineSensorReadArray(tLineSensor *ls, float *array) {
    int i;
    
    // Check if we need to read a value
    if (!ls->in_callback && !ls->continous) {
        // Just call LineSensorBackgroundRead and busy wait
        LineSensorBackgroundRead(ls, 0, 0);
        while (ls->pending);
    }
    
    // Check for any errors
    if (!I2CSuccess(ls->i2c)) {
        for (i=0; i < 8; i++)
            array[i] = INFINITY;
    }
    
    // Calculate the values
    for (i=0; i < 8; i++) {
        array[i] = (ls->values[i] / (float)(0x100));
    }
}


// Handler for reading a single set of values
static void SingleReadHandler(tLineSensor *ls) {
    LineSensorBackgroundRead(ls, 0, 0);
}

// Handler for reading as fast as possible
static void ContinuousReadHandler(tLineSensor *ls) {
    LineSensorBackgroundRead(ls, ContinuousReadHandler, ls);
}

// These function set up a LineSensor to read indefinitly
// Any following calls to LineSensorRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the LineSensor to complete, the LineSensor will read as fast as possible without overlap
void LineSensorReadContinuouslyUS(tLineSensor *ls, tTime us) {
    // Set the continous flag
    ls->continous = true;
    
    // Check if there isn't enough time for the sensor to be read
    // NOTE: This is unknown at the time and may be calculatable
    // For now only a time of 0 will switch to the fast as possible solution
    if (us <= 1)
        // If there isn't, read as fast as possible
        LineSensorBackgroundRead(ls, ContinuousReadHandler, ls);
    else
        // Otherwise just periodically call the single read handler
        CallEveryUS(SingleReadHandler, ls, us);
}

void LineSensorReadContinuously(tLineSensor *ls, float s) {
    LineSensorReadContinuouslyUS(ls, US(s));
}





