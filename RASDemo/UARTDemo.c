#include <StellarisWare/inc/hw_types.h>		// tBoolean
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/utils/uartstdio.h>	// input/output over UART
#include <StellarisWare/driverlib/uart.h>		// input/output over UART
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/sysctl.h>

#include "RASDemo.h"

void initUART(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
}

void uartDemo(void) {
	UARTprintf("tell me something!\n-> ");
	
	{
		char charArray[100];
		UARTgets(charArray, 100);
		UARTprintf("you said, \"%s\"\n", charArray);
		UARTprintf("thanks for the input!\n");
	}
	
	{
		char newline = 13;
		char ch = getc();
		while(ch != newline) {
			ch = getc();
			putc(ch);
		}
	}
}

char getc(void) {
	char ch = UARTgetc();
	return ch;
}

void putc(char ch) {
	UARTprintf("%c",ch);
}

int keyWasPressed(void) {
	if (UARTCharsAvail(UART0_BASE))	
		return 1;
	else
		return 0;
}
