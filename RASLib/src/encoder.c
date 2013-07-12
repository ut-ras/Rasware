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
    unsigned long ulInterrupt;
    
    // Recorded number of encoder "ticks" thus far
    signed long slTicks;
}tEncoder;


unsigned long AddEncoder(unsigned long ulPort, unsigned char ucPinA, unsigned char ucPinB, unsigned long ulInterrupt){
    static unsigned long encoder_count = 0;
    SysCtlPeripheralEnable(ulPort);
    GPIOPinTypeGPIOInput(ulPort, (1<<ucPinA) | (1<<ucPinB));
    
    IntEnable(ulInterrupt);
    return encoder_count++;
}

