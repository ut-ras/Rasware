#include <StellarisWare/inc/hw_types.h>		// tBoolean
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/utils/uartstdio.h>	// input/output over UART
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/sysctl.h>

#include <RASLib/inc/encoder.h>

#include "RASDemo.h"

void initEncoders(void) {
	InitializeEncoders(false, false);
}

void encoderDemo(void) {
	UARTprintf("Press:\nany key-read encoder values\n");
	UARTprintf("any key after read begins-quit\n");
	
	{
		signed long encoder0, encoder1, counter = 0;
		PresetEncoderCounts(0, 0);

		while(!keyWasPressed()) {	  
			encoder0 = GetEncoderCount(ENCODER_0);	 
			encoder1 = GetEncoderCount(ENCODER_1);
			GetEncoderCounts(&encoder0, &encoder1);
			UARTprintf("enc0:%d  enc1:%d      \r",encoder0,encoder1);
			counter++;
		}
	}

	UARTprintf("\n");
}
