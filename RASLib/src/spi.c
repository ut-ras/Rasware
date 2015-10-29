//*****************************************************************************
//
// spi - Serial Peripheral Interface
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

#include "spi.h"
#include "gpio.h"
#include "time.h"

#include <stdint.h>
#include <stddef.h>
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_types.h>
#include <StellarisWare/inc/hw_ssi.h>
#include <StellarisWare/inc/hw_ints.h>
#include <StellarisWare/driverlib/ssi.h>
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/driverlib/sysctl.h>


struct SPI {
        const uint32_t base;
        uint32_t frame_mask;
        tBoolean invert_csl;
};

static tSPI spiBuffer[SPI_COUNT] = {
        {SSI0_BASE},
        {SSI1_BASE},
        {SSI2_BASE},
        {SSI3_BASE},
};

static const int8_t spiLookupByCLK[PIN_COUNT] = {
        [PIN_A2] = 0,
        [PIN_F2] = 1,
        [PIN_B4] = 2,
        [PIN_D0] = 3,
        [0] = -1,
};

static const tPin indexToMOSI[SPI_COUNT] = {
        PIN_A5, PIN_F1, PIN_B7, PIN_D3
};

static const tPin indexToMISO[SPI_COUNT] = {
        PIN_A4, PIN_F0, PIN_B6, PIN_D2
};

static const uint32_t indexToSysctl[SPI_COUNT] = {
        SYSCTL_PERIPH_SSI0,
        SYSCTL_PERIPH_SSI1,
        SYSCTL_PERIPH_SSI2,
        SYSCTL_PERIPH_SSI3,
};

static const uint32_t indexToPort[SPI_COUNT] = {
        GPIO_PORTA_BASE,
        GPIO_PORTF_BASE,
        GPIO_PORTB_BASE,
        GPIO_PORTD_BASE
};

static const uint8_t indexToPins[SPI_COUNT] = {
        GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5,
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2,
        GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3
};

static const uint32_t indexToCLKcfg[SPI_COUNT] = {
        GPIO_PA2_SSI0CLK,
        GPIO_PF2_SSI1CLK,
        GPIO_PB4_SSI2CLK,
        GPIO_PD0_SSI3CLK
};

static const uint32_t indexToMOSIcfg[SPI_COUNT] = {
        GPIO_PA5_SSI0TX,
        GPIO_PF1_SSI1TX,
        GPIO_PB7_SSI2TX,
        GPIO_PD3_SSI3TX
};
static const uint32_t indexToMISOcfg[SPI_COUNT] = {
        GPIO_PA4_SSI0RX,
        GPIO_PF0_SSI1RX,
        GPIO_PB6_SSI2RX,
        GPIO_PD2_SSI3RX
};

tSPI *InitializeSPI(tPin clk, tPin mosi, tPin miso, int frequency,
                    int bits_per_frame, tBoolean clock_polarity,
                    tBoolean phase){
        int spiIndex = spiLookupByCLK[clk];
        tSPI * toRet;
        uint32_t crap;
        if (spiIndex == -1) return NULL;
        if (indexToMISO[spiIndex] != miso) return NULL;
        if (indexToMOSI[spiIndex] != mosi) return NULL;

        toRet = &spiBuffer[spiIndex];
        toRet->frame_mask = (1 << bits_per_frame) - 1;
        toRet->invert_csl = clock_polarity;

        SysCtlPeripheralEnable(indexToSysctl[spiIndex]);
        GPIOPinConfigure(indexToCLKcfg[spiIndex]);
        GPIOPinConfigure(indexToMOSIcfg[spiIndex]);
        GPIOPinConfigure(indexToMISOcfg[spiIndex]);
        GPIOPinTypeSSI(indexToPort[spiIndex], indexToPins[spiIndex]);
        SSIConfigSetExpClk(toRet->base, SysCtlClockGet(),
                           ((phase) ? 0x2 : 0) | ((clock_polarity) ? 0x1 : 0),
                           SSI_MODE_MASTER, frequency, bits_per_frame);
        SSIEnable(toRet->base);
        /* clean up the recieve fifo */
        while(SSIDataGetNonBlocking(toRet->base, &crap));
        return toRet;
}

tBoolean SPIRequestUS(tSPI *spi, tPin csl,
                    const uint32_t *sendData, unsigned int sendLen,
                    uint32_t *recData, unsigned int recLen,
                    tTime wait) {
        int i;
        if (sendLen < 1 && recLen < 1) return true;
        if (csl > 0) SetPin(csl, !spi->invert_csl);
        WaitUS(wait);
        for (i = 0; i < sendLen || i < recLen; ++i) {
                SSIDataPut(spi->base, sendData[i]);
                if (recLen > i) SSIDataGet(spi->base, &recData[i]);
                WaitUS(wait);
        }
        while (SSIBusy(spi->base));
        if (csl > 0) SetPin(csl, spi->invert_csl);
        return true;
}

tBoolean SPIRequest(tSPI *spi, tPin csl,
                    const uint32_t *sendData, unsigned int sendLen,
                    uint32_t *recData, unsigned int recLen,
                    float wait) {
        SPIRequestUS(spi, csl, sendData, sendLen, recData, recLen, US(wait));
}
