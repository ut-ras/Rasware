
//*****************************************************************************
//
// spi - Serial Peripheral Interface driver
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

#ifndef _R_SPI_H_
#define _R_SPI_H_

#include "common.h"
#include "gpio.h"
#include "time.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Definition of struct SPI in spi.c
        typedef struct SPI tSPI;

// Number of available SPI modules
#define SPI_COUNT 4

/**
 * Initializes a SPI module on a trio of pins
 * @param clk clock pin
 * @param mosi master out slave in
 * @param miso master in slave out
 * @param frequency [TODO: self explanitory]
 * @param bits_per_frame [TODO: self explanitory]
 * @param clock_polarity [TODO: explain]
 * @param phase [TODO: explain]
 * @return Pointer to a SPI module used by other spi functions
 */
        tSPI *InitializeSPI(tPin clk, tPin mosi, tPin miso, int frequency,
                            int bits_per_frame, tBoolean clock_polarity,
                            tBoolean phase);

/**
 * Requests data from a SPI device (blocking).
 * @param spi An initialized SPI module, which is returned by InitializeSPI
 * @param csl A chip select line
 * @param sendData Pointer to array of byes being sent
 * @param sendLen Number of bytes allocated in the send data array
 * @param recData Pointer to array of byes to store the data received
 * @param recLen Number of bytes allocated in the receive data array
 * @param wait Time between frames sent over spi
 * @return true if successful
 */
        tBoolean SPIRequest(tSPI *spi, tPin csl,
                            const uint32_t *sendData, unsigned int sendLen,
                            uint32_t *recData, unsigned int recLen,
                            float wait);
/**
 * Requests data from a SPI device (blocking).
 * @param spi An initialized SPI module, which is returned by InitializeSPI
 * @param csl A chip select line
 * @param sendData Pointer to array of byes being sent
 * @param sendLen Number of bytes allocated in the send data array
 * @param recData Pointer to array of byes to store the data received
 * @param recLen Number of bytes allocated in the receive data array
 * @param wait Time between frames sent over spi in US
 * @return true if successful
 */
        tBoolean SPIRequestUS(tSPI *spi, tPin csl,
                              const uint32_t *sendData, unsigned int sendLen,
                              uint32_t *recData, unsigned int recLen,
                              tTime wait);

#ifdef __cplusplus
}
#endif

#endif // _R_SPI_H_
