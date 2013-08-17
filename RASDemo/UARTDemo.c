/* suppressed to make room for uart.h
#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over 
#include <driverlib/uart.h>		// input/output over 
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include "RASDemo.h"

void init(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
    GPIOPinType(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	StdioInit(0);
}

void uartDemo(void) {
	printf("tell me something!\n-> ");
	
	{
		char charArray[100];
		gets(charArray, 100);
		printf("you said, \"%s\"\n", charArray);
		printf("thanks for the input!\n");
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

void putc(char ch) {
	printf("%c",ch);
}

int keyWasPressed(void) {
	if (CharsAvail(0_BASE))	
		return 1;
	else
		return 0;
}
*/
