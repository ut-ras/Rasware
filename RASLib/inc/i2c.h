// ***************************
// I2C FUNCTIONS AND DEFINES
// ***************************
#ifndef I2C_H
#define I2C_H

#include "internal.h"
#include "gpio.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif
    
// Definition of struct I2C in i2c.c
typedef struct I2C tI2C;

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
                                  unsigned char *data, unsigned int len,
                                  tCallback callback, void *cbdata);

// This function sends data to an I2C addresss
// Takes a pointer to an array to send from
// Returns true if successful
tBoolean I2CSend(tI2C *i2c, unsigned char addr, 
                            unsigned char *data, unsigned int len);

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

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // I2C_H
