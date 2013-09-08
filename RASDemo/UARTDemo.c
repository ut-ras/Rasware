#include "RASDemo.h"

#include <RASLib/inc/uart.h>

void uartDemo(void) {
	Printf("tell me something!\n-> ");
	
	{
		char charArray[100];
		Gets(charArray, 100);
		Printf("you said, \"%s\"\n", charArray);
		Printf("thanks for the input!\n");
	}
	
	{
		char newline = 13;
		char ch = Getc();
		while(ch != newline) {
			ch = Getc();
			Putc(ch);
		}
	}
}
