#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/encoder.h>

#include "RASDemo.h"

tEncoder *demoEncoder[2];

void initEncoders(void) {
    demoEncoder[0] = InitializeEncoder(PIN_B0, PIN_B1);
    demoEncoder[1] = InitializeEncoder(PIN_B2, PIN_B3);
}

void encoderDemo(void) {
	// 2012 Encoder Demo Code
	UARTprintf("Press:\nany key-read encoder values\n");
	UARTprintf("any key after read begins-quit\n");
    
    ResetEncoder(demoEncoder[0]);
    ResetEncoder(demoEncoder[1]);

    while(!keyWasPressed()) {
        UARTprintf("enc0:%d  enc1:%d      \r",
                   GetEncoder(demoEncoder[0]),
                   GetEncoder(demoEncoder[1]) );
	}
    
	UARTprintf("\n");
	
	UARTprintf("Encoder Demo not working.\n");
}
