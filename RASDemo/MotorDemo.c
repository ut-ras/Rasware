#include <StellarisWare/hw_types.h>		// tBoolean
#include <StellarisWare/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/motor.h>

#include "RASDemo.h"

void initMotors(void) {
	InitializeMotors(false, false);
}

void motorDemo(void) {
	UARTprintf("Press:\n  w-forward\n  s-backward\n  a-left\n  ");
	UARTprintf("d-right\n  space-stop\n  enter-quit\n");
	
	{
		// values should range between -128 and 127?
		signed char left = 0, right = 0, maxSpeed = 127;
		char newline = 13;
		char ch = getc();
		while(ch != newline) {
			ch = getc();
			putc(ch);
			if (ch == 'w') {
				left = maxSpeed;
				right = -maxSpeed;
			} else if (ch == 's') {
				left = -maxSpeed;
				right = maxSpeed;
			} else if (ch == 'a') {
				left = -maxSpeed;
				right = -maxSpeed;
			} else if (ch == 'd') {
				left = maxSpeed;
				right = maxSpeed;
			} else if (ch == ' ') {
				left = 0;
				right = 0;
			}
			SetMotorPowers(left, right);
		}
	}
	
	SetMotorPowers(0,0);
	UARTprintf("\n");
}
