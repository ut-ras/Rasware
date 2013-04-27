
#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "init.h" 
#include "uart.h" 

int main(void) {
	// LockoutProtection();
	InitializeMCU();
	
	InitializeUART();
	
	UARTprintf("\nRAS Demo for Robotathon 2013\n");
	
	while(1);
}
