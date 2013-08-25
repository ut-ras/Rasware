
#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include "RASDemo.h"
#include <RASLib/inc/uart.h>

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
			printf("%c",ch);
		}
	}
}
