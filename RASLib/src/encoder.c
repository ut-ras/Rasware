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
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"


/***************** BEGIN DEFINITION OF QUADRATURE DECODER (MEALY MACHINE) *****************/
struct EncoderState{
    int Out[4]; //output due to change in state
};
typedef const struct EncoderState tEncoderState;
static const tEncoderState QuadratureDecoder[4] = {
    {0,1,-1,0}, //State 00
    {-1,0,0,1}, //State 01
    {1,0,0,-1}, //State 10
    {0,-1,1,0}  //State 11
};
static const tEncoderState* Next[4] = {&QuadratureDecoder[0],&QuadratureDecoder[1],&QuadratureDecoder[2],&QuadratureDecoder[3]};
typedef tEncoderState* tDecoderFSM;
static int DriveQuadratureDecoder(tDecoderFSM enc, unsigned long input){
    // Store the output value as instructed by the FSM
    int output = enc->Out[input];
    
    // Move the state pointer to the input value's position
    enc = Next[input];
    
    return output;
}
typedef struct{
    // Pointer to what state the encoder is in
    tDecoderFSM decoder;
    
    // Data associated with what port/pin the encoder is plugged into
    unsigned long ulPort;
    unsigned char ucPinA;
    unsigned char ucPinB;
    
    // Invert
    signed char invert;
    
    // Recorded number of encoder "ticks" thus far
    signed long slTicks;
}tEncoder;
/***************** END DEFINITION OF QUADRATURE DECODER (MEALY MACHINE) *****************/


static tEncoder Encoders[ENCODER_BUFFER_SIZE]; // change this to be dynamicly allocated eventually. buffer size of 4 for now.

// A generic handler for all encoders
// To be called on any possible encoder event
void EncoderHandler(void){
    tEncoder* enc = Encoders;
    
    // Check if it's at an active encoder
    while(enc->decoder == &QuadratureDecoder[0] ||
          enc->decoder == &QuadratureDecoder[1] ||
          enc->decoder == &QuadratureDecoder[2] ||
          enc->decoder == &QuadratureDecoder[3] )
    {
        unsigned long input;
        
        // Load what the physical pins read
        input = GPIOPinRead(enc->ulPort, (1<<enc->ucPinA) | (1<<enc->ucPinB));
        
        // Move PinA to Bit0 and PinB to Bit1
        input = ((input & (1<<enc->ucPinA))>>enc->ucPinA) | (((input & (1<<enc->ucPinB))>>enc->ucPinB)<<1);
        
        // Move the state machine and add it's output to the recorded ticks 
        enc->slTicks += DriveQuadratureDecoder(enc->decoder, input);
        
        // Move to the next encoder
        enc = &enc[1];
    }
}

int AddEncoder(unsigned long ulPeripheral, unsigned long ulPort, unsigned char ucPinA, unsigned char ucPinB, unsigned long ulInterrupt, tGPIO_Port port){
    static unsigned long encoder_count = 0;
    
    // Returns -1 (error) if buffer is full
    if(encoder_count == ENCODER_BUFFER_SIZE) return -1;
    
    // Loads a new encoder module into the Encoders buffer
    Encoders[encoder_count].decoder = QuadratureDecoder;
    Encoders[encoder_count].ulPort = ulPort;
    Encoders[encoder_count].ucPinA = ucPinA;
    Encoders[encoder_count].ucPinB = ucPinB;
    Encoders[encoder_count].slTicks = 0;
    
    // Enable SysCtrl for the specified port
    SysCtlPeripheralEnable(ulPeripheral);
    
    // Sets the specified pins as input pins
    GPIOPinTypeGPIOInput(ulPort, (1<<ucPinA) | (1<<ucPinB));
    
    // Attach EncoderHandler to the specified port
    AttachToInterruptHandler(port,EncoderHandler);
    
    // Enable hardware interrupts to the specified pins
    GPIOPinIntEnable(ulPort, (1<<ucPinA) | (1<<ucPinB));
    IntEnable(ulInterrupt);
    
    // Returns the index of the newly assigned encoder
    return encoder_count++;
}

// Retrieves encoder ticks thus far by index
// Encoders are indexed by the order in which they were added
signed long GetEncoderTicks(unsigned char index){
    return Encoders[index].slTicks;
}

