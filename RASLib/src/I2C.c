//*******************************
// I2C Functions and Defines 
//*******************************

#include <stdlib.h>
#include <stdarg.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/i2c.h>
#include "i2c.h"
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <inc/hw_i2c.h>

static unsigned char i2c_buffer[32];


void I2CInit(void)
{ 
	  // Only for use of pin PB2 and PB3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    
    I2CMasterEnable(I2C0_MASTER_BASE);
}
// Summary:	Sends 'num' number of characters to specified address
// Parameters:
//		addr:	address to send data to
//		num:	number of characters being sent
//		...:	characters to send, separated by commas
// Note:	Number of characters must be equal to 'num'
void I2CSend(unsigned short addr, int num, ...)
{
	// Make sure data is actually being sent
	if (num > 0 && num < 32){   // Max size of buffer
		// Allocate memory for data
		unsigned char *data = i2c_buffer;
		va_list args;
		int i=0;
		
		// Put characters to send in array
		va_start(args, num);
		for(; i<num; i++)
			data[i] = (unsigned char) va_arg(args, int);
		va_end(args);
	
		I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, addr >> 1, false);
		I2CMasterDataPut(I2C0_MASTER_BASE, *data);
		if (num == 1){
		    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
			while(I2CMasterBusy(I2C0_MASTER_BASE));
			return;
		}
		
		// Start sending consecutive data
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
		num--;
		data++;
		
		// Continue sending consecutive data
		while(num > 1){
			I2CMasterDataPut(I2C0_MASTER_BASE, *data);
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
			while(I2CMasterBusy(I2C0_MASTER_BASE));
			num--;
			data++;
		}
		
		// Send last piece of data
		I2CMasterDataPut(I2C0_MASTER_BASE, *data);
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
	}
}

// Summary:	Recieve/Fetch data from specified address
// Parameters:
//		addr:	address to recieve data from
//		data:	pointer to memory location to save data
//		len:	number of cahracers that will be recieved
void I2CRecieve(unsigned short addr, unsigned char* data, unsigned int len)
{
    if (len < 1)	// Assume I2C Recieving will always return data
        return;
	
	// Set address to read from
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, addr >> 1, true);
	
	// Check to see if pointer is to an array
	if (len == 1){
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
		*data = I2CMasterDataGet(I2C0_MASTER_BASE);
		return;
	}
	
	// Begin reading consecutive data
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C0_MASTER_BASE));
	*data = I2CMasterDataGet(I2C0_MASTER_BASE);
	len--;
	data++;
	
	// Continue reading consecutive data
	while(len > 1){
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    	while(I2CMasterBusy(I2C0_MASTER_BASE));
		*data = I2CMasterDataGet(I2C0_MASTER_BASE);
		len--;
		data++;
	}
	
	// Read last character of data	
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_MASTER_BASE));
	*data = I2CMasterDataGet(I2C0_MASTER_BASE);
}





