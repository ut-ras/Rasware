#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/motor.h>

#include "RASDemo.h"

AddMotor(Left,A,4,A,5,COAST)
AddMotor(Right,A,6,A,7,COAST)

void initMotors(void) {
	InitializeMotorLeft();
	InitializeMotorRight();
}

void motorDemo(void) {
	// 2012 Motor Demo
	UARTprintf("Press:\n  w-forward\n  s-backward\n  a-left\n  ");
	UARTprintf("d-right\n  space-stop\n  enter-quit\n");
	
	{
		// max speed is 1.0 but 0.75 lets us check for working pwm
		float left = 0, right = 0, speed = 0.75;
		char newline = 13;
		char ch = getc();
		while(ch != newline) {
			ch = getc();
			putc(ch);
			if (ch == 'w') {
				left = speed;
				right = -speed;
			} else if (ch == 's') {
				left = -speed;
				right = speed;
			} else if (ch == 'a') {
				left = -speed;
				right = -speed;
			} else if (ch == 'd') {
				left = speed;
				right = speed;
			} else if (ch == ' ') {
				left = 0;
				right = 0;
			}
			SetMotorLeft(left);
			SetMotorRight(right);
			UARTprintf("Set Motor to 0x%x%x 0x%x%x\n", left, right);
		}
	}
	
	SetMotorLeft(0);
	SetMotorRight(0);
	UARTprintf("\n");
}
