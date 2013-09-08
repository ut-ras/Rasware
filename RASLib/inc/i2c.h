//*****************************************************************************
//
// i2c - Inter-Intergrated Circuit driver
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

#ifndef _R_I2C_H_
#define _R_I2C_H_

#include "common.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


// Constants used for I2C timing
// Each is given in units of microseconds
#define I2C_TIMEOUT 20000
    
// Definition of struct I2C in i2c.c
typedef struct I2C tI2C;

// Number of available I2C modules
#define I2C_COUNT 6

// Function to initialize an I2C module on a pair of pins
// The returned pointer can be used by the Send and Recieve functions
tI2C *InitializeI2C(tPin sda, tPin scl);
    
// This function returns true if the 
// previous transaction was successful
tBoolean I2CSuccess(tI2C *i2c);
    
// This function sends data to an I2C address.
// A callback can be passed and will be called when 
// all of the data in the passed array is sent.
void I2CBackgroundSend(tI2C *i2c, unsigned char addr, 
                                  const unsigned char *data, unsigned int len,
                                  tCallback callback, void *cbdata);

// This function sends data to an I2C addresss
// Takes a pointer to an array to send from
// Returns true if successful
tBoolean I2CSend(tI2C *i2c, unsigned char addr, 
                            const unsigned char *data, unsigned int len);

// This function receives data from an I2C address.
// A callback can be passed and will be called when 
// all of the data is loaded into the passed array.
void I2CBackgroundReceive(tI2C *i2c, unsigned char addr, 
                                     unsigned char *data, unsigned int len,
                                     tCallback callback, void *cbdata);
    
// This function sends data to an I2C addresss
// Takes a pointer to an array to receive to
// Returns true if successful
tBoolean I2CReceive(tI2C *i2c, unsigned char addr, 
                               unsigned char* data, unsigned int len);
                               
// This function requests data from an I2C address.
// A callback can be passed and will be called when 
// all of the data is loaded into the passed array.
void I2CBackgroundRequest(tI2C *i2c, unsigned char addr, 
                                     const unsigned char *sendData, unsigned int sendLen,
                                     unsigned char *recData, unsigned int recLen,
                                     tCallback callback, void *cbdata);
    
// This function requests data from an I2C address.
// Takes two pointers to arrays. The first is the data to send
// and the second is to hold the data recieved. 
// This is the same as two sequential send and recieve calls
// but takes place in the internal state machine.
// Returns true if successful
tBoolean I2CRequest(tI2C *i2c, unsigned char addr, 
                               const unsigned char *sendData, unsigned int sendLen,
                               unsigned char *recData, unsigned int recLen);


#ifdef __cplusplus
}
#endif

#endif // _R_I2C_H_
