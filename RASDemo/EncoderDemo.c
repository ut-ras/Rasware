#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/encoder.h>

#include "RASDemo.h"

void initEncoders(void) {
	//InitializeEncoders(false, false);
	UARTprintf("Encoder init not working.\n");
	//Replace with InitialieEncoder calls
}

void encoderDemo(void) {
	/* 2012 Encoder Demo Code
	UARTprintf("Press:\nany key-read encoder values\n");
	UARTprintf("any key after read begins-quit\n");{
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
	UARTprintf("\n");*/
	
	UARTprintf("Encoder Demo not working.\n");
}
