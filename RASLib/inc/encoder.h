//*****************************************************************************
//
// encoder - Quadrature Encoder driver
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

#ifndef _R_ENCODER_H_
#define _R_ENCODER_H_

#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


// Definition of struct Encoder in encoder.c
typedef struct Encoder tEncoder;

// Function to initialize an encoder on a pair of pins
// The returned pointer can be used by the GetEncoder function
tEncoder *InitializeEncoder(tPin a, tPin b, tBoolean invert);

// This function returns the accumulated encoder value
signed long GetEncoder(tEncoder *enc);

// This function clears any ticks on the encoder
void ResetEncoder(tEncoder *enc);


#ifdef __cplusplus
}
#endif

#endif // _R_ENCODER_H_
