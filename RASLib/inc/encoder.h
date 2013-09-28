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

/**
 * Initializes an encoder on a pair of pins
 * @param a Pin that the encoder A line should be plugged into
 * @param b Pin that the encoder B line should be plugged into
 * @param invert Flag to switch the direction that the encoder value is incremented 
 * @return Pointer to a tEncoder used by the GetEncoder and ResetEncoder functions
 */
tEncoder *InitializeEncoder(tPin a, tPin b, tBoolean invert);

/**
 * Gets the accumulated value for an encoder
 * @param enc Pointer to a tEncoder that was returned by the InitializeEncoder function
 * @return Accumulated encoder value
 */
signed long GetEncoder(tEncoder *enc);

/**
 * Clears any ticks that have been accumulated for an encoder
 * @param enc Pointer to a tEncoder that was returned by the InitializeEncoder function
 */
void ResetEncoder(tEncoder *enc);


#ifdef __cplusplus
}
#endif

#endif // _R_ENCODER_H_
