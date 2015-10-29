//*****************************************************************************
//
// encoder.c - encoder driver
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

#include "encoder.h"


// Definition of tDecoderState
// Contains all data for the mealy machine
typedef const struct {
    int value[4];
} tDecoderState;

// The states of the mealy machine
static const tDecoderState DecoderFSM[4] = {
  {{ 0, 1,-1, 0}}, //State 00
  {{-1, 0, 0, 1}}, //State 01
  {{ 1, 0, 0,-1}}, //State 10
  {{ 0,-1, 1, 0}}  //State 11
};

// Definition of struct Encoder
// Defined to tEncoder in encoder.h
struct Encoder {
    // Pointer to what state the encoder is in
    tDecoderState *decoder;
    
    // Data associated with what pins the encoder is plugged into
    tPin pinA;
    tPin pinB;
    
    // Recorded number of encoder "ticks" thus far
    signed long ticks;

    // Set to switch direction encoder is incremented
    tBoolean invert;
};

// Buffer of encoder structs to use
// There can only be the total count of pins/2 since each
// encoder needs 2 pins
tEncoder encoderBuffer[PIN_COUNT / 2];

int encoderCount = 0;


// Handler to respond to pin interrupts
static void EncoderHandler(void *data) {
    tEncoder *enc = data;
    
    // Determine actual value of pins
    // and normalize inputs for lookup in a table
    int input = (GetPin(enc->pinA) ? 0x1 : 0x0) |
                (GetPin(enc->pinB) ? 0x2 : 0x0);
    
    // Add the value in the FSM to the current tick count
    // (or substract if invert is set)
    if (enc->invert) {
        enc->ticks -= enc->decoder->value[input];
    } else {
        enc->ticks += enc->decoder->value[input];
    }

    // Setup the next state of the mealy machine
    enc->decoder = &DecoderFSM[input];
}

// Function to initialize an encoder on a pair of pins
// The returned pointer can be used by the GetEncoder function
tEncoder *InitializeEncoder(tPin a, tPin b, tBoolean invert) {
    // Grab the next encoder
    tEncoder *enc = &encoderBuffer[encoderCount++];
    
    // Setup the initial data
    enc->decoder = &DecoderFSM[0];
    enc->pinA = a;
    enc->pinB = b;
    enc->ticks = 0;
    enc->invert = invert;

    // Register the interrupt handler on the pins
    CallOnPin(EncoderHandler, enc, a);
    CallOnPin(EncoderHandler, enc, b);
    
    // Return the new encoder
    return enc;
}

// This function returns the accumulated encoder value
signed long GetEncoder(tEncoder *enc) {
    return enc->ticks;
}

// This function clears any ticks on the encoder
void ResetEncoder(tEncoder *enc) {
    enc->ticks = 0;
}
