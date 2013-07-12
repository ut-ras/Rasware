#include <utils/uartstdio.h>	// input/output over UART
#include <RASLib/inc/servo.h>
#include "RASDemo.h"
#define SERVO_0 	0
#define SERVO_1		1
#define SERVO_2		2
#define	SERVO_3		3

void initServo(void) {
	//InitializeServoGenerator();
}

void servoDemo(void) {
	UARTprintf("Press:\n  a-'up' 10\n  w-'up' 1\n  s-'down' 1\n");
	UARTprintf("  d-'down' 10\n  enter-quit\n");
	
	{
		unsigned char position = 0;
		char newline = 13;
		char ch;
		
		UARTprintf("position: %u  ",position);
		ch = getc();
		while(ch != newline) {
			if (ch == 'w')
				position++;
			else if (ch == 's')
				position--;	 
			else if (ch == 'a')
				position+=10;
			else if (ch == 'd')
				position-=10;
			SetServoPosition(SERVO_0, position); 
			SetServoPosition(SERVO_1, position);
			SetServoPosition(SERVO_2, position);
			SetServoPosition(SERVO_3, position);
			UARTprintf("%u  ",position);	 
			ch = getc();
		}
				 
		UARTprintf("\n");
	}
}
