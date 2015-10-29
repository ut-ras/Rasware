//*****************************************************************************
//
// common - Shared functionality
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

#ifndef _R_COMMON_H_
#define _R_COMMON_H_

// Included for definition of tBoolean
#include <StellarisWare/inc/hw_types.h>

// Included for common use in debugging
#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif


// Definition of callback function pointer
typedef void (*tCallback)();

/**
 * Function which does nothing
 */
void Dummy(void);
    
/**
 * Sets up the LM4F to use RASLib
 */
void InitializeMCU(void);
    
/**
 * Set up a function to be called on hard-fault.
 * Defaults to setting the red LED on.
 */
void OnHardFault(tCallback cb, void *data);


#ifdef __cplusplus
}
#endif

#endif // _R_COMMON_H_
