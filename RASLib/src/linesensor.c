#include "linesensor.h"
#include "i2c.h"
#include "UART.h"
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

#define ADS7830 0x48
#define ADS7830_REQ 0x84

static tI2C *i2c;

void initLineSensor()
{
	i2c = InitializeI2C(PIN_B3, PIN_B2);	
}

// Grabs line sensor data using i2c and dumps into LineSensor array. 
// Returns 1 if successful, 0 if encountered a timeout error 
//	(timeout probably means the line sensor isn't connected correctly)
int readLineSensor(unsigned char *data)
{
	int i;
	int err; 
	unsigned char cmd = ADS7830_REQ; //1 CH# 01 XX for request conversion. e.g 1 000 01 00 is for channel 2

	for(i=0; i<8; i++)
	{
		I2CSend(i2c, ADS7830, &cmd, 1); 
		err = I2CMasterErr(I2C0_MASTER_BASE);
		if(err != 0) 
          {
            UARTprintf("Error:%d in i2c send",err); 
          }
		I2CReceive(i2c, ADS7830,&data[i],1);
		err = I2CMasterErr(I2C0_MASTER_BASE);
		if(err != 0)
		{ UARTprintf("Error:%d in i2c recieve",err); }
		cmd += 0x10; 
	} 

	return 1;
}
