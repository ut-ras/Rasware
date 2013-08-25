// ***************************
// I2C FUNCTIONS AND DEFINES
// ***************************
#ifndef I2C_H
#define I2C_H

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

// Summary:	Sends 'len' number of characters to specified address
// Parameters:
//		addr:	address to send data to
//      data:   pointer to memory location to read data
//		len:	number of characters being sent
// Note:	Number of characters must be equal to 'len'
extern void I2CSend(tI2C *i2c, unsigned short addr, unsigned char *data, int len);

// Summary:	Recieve/Fetch data from specified address
// Parameters:
//		addr:	address to recieve data from
//		data:	pointer to memory location to save data
//		len:	number of cahracers that will be recieved
extern void I2CRecieve(tI2C *i2c, unsigned short addr, unsigned char* data, int len);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // I2C_H
