#include "RASDemo.h"
#include <RASLib/inc/gpio.h>

#include <RASLib/inc/common.h>

void gpioDemo(void) {
//	char charArray[100];
	tBoolean PinValue = false;
	char newline = 13;
	char ESC = 27;
	tPin OutPutPin = PIN_A2, InputPin = PIN_A3;
//	Printf("Enter GPIO pin [Ex. A3] to set high/low [toggle with button 0]\n");
//	Gets(charArray, 100);
	char ch = 0;




	while(ch != ESC) {
		if(ch == newline) {
			SetPin(OutPutPin, true);
		} else {
		 	SetPin(OutPutPin, false);
		}
		PinValue = GetPin(InputPin);
		Printf("InputPin: %i\n",PinValue);
		ch = Getc();
	}
}
