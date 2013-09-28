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

/**
 * Initializes an I2C module on a pair of pins
 * @param sda [TODO: describe sda]
 * @param scl [TODO: describe scl]
 * @return Pointer to a I2C module used by other i2c functions
 */
tI2C *InitializeI2C(tPin sda, tPin scl);

/**
 * Checks if the previous i2c transaction was successful
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @return true if the previous i2c transaction was successful
 */
tBoolean I2CSuccess(tI2C *i2c);
    
/**
 * Sends data to an I2C address
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @param addr I2C address where data is being sent
 * @param data Pointer to array of byes being sent
 * @param len Number of bytes in data array
 * @param callback Function that will be called when the data has been sent
 * @param cbdata Argument sent to the callback whenever it is called
 */
void I2CBackgroundSend(tI2C *i2c, unsigned char addr, 
                                  const unsigned char *data, unsigned int len,
                                  tCallback callback, void *cbdata);

/**
 * Sends data to an I2C address
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @param addr I2C address where data is being sent
 * @param data Pointer to array of byes being sent
 * @param len Number of bytes in data array
 * @return true if successful
 */
tBoolean I2CSend(tI2C *i2c, unsigned char addr, 
                            const unsigned char *data, unsigned int len);

/**
 * Receives data from an I2C address
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @param addr I2C address where data is being received
 * @param data Pointer to array of byes to store the data received
 * @param len Number of bytes allocated in the data array
 * @param callback Function that will be called when all of the data is received
 * @param cbdata Argument sent to the callback whenever it is called
 */
void I2CBackgroundReceive(tI2C *i2c, unsigned char addr, 
                                     unsigned char *data, unsigned int len,
                                     tCallback callback, void *cbdata);
    
/**
 * Receives data from an I2C address
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @param addr I2C address where data is being received
 * @param data Pointer to array of byes to store the data received
 * @param len Number of bytes allocated in the data array
 * @return true if successful
 */
tBoolean I2CReceive(tI2C *i2c, unsigned char addr, 
                               unsigned char* data, unsigned int len);
                               
/**
 * Requests data from an I2C address (nonblocking). This is the same as two sequential send and recieve calls but takes place in the internal state machine.
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @param addr I2C address where data is being requested
 * @param sendData Pointer to array of byes being sent
 * @param sendLen Number of bytes allocated in the send data array
 * @param recData Pointer to array of byes to store the data received
 * @param recLen Number of bytes allocated in the receive data array
 * @param callback Function that will be called when all of the data is received
 * @param cbdata Argument sent to the callback whenever it is called
 */
void I2CBackgroundRequest(tI2C *i2c, unsigned char addr, 
                                     const unsigned char *sendData, unsigned int sendLen,
                                     unsigned char *recData, unsigned int recLen,
                                     tCallback callback, void *cbdata);
    
/**
 * Requests data from an I2C address (blocking). This is the same as two sequential send and recieve calls but takes place in the internal state machine.
 * @param i2c An initialized I2C module, which is returned by InitializeI2C
 * @param addr I2C address where data is being requested
 * @param sendData Pointer to array of byes being sent
 * @param sendLen Number of bytes allocated in the send data array
 * @param recData Pointer to array of byes to store the data received
 * @param recLen Number of bytes allocated in the receive data array
 * @return true if successful
 */
tBoolean I2CRequest(tI2C *i2c, unsigned char addr, 
                               const unsigned char *sendData, unsigned int sendLen,
                               unsigned char *recData, unsigned int recLen);


#ifdef __cplusplus
}
#endif

#endif // _R_I2C_H_
