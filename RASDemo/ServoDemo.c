#include <utils/uartstdio.h>	// input/output over UART
#include <RASLib/inc/servo.h>
#include "RASDemo.h"
#include "time.h"
#include "servo.h"

AddServo(One,A,2)
void initServo(void) {
	//InitializeServoGenerator();
		InitializeServoOne();
}

void servoDemo(void) {
	/* 2012 Servo Demo code
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
	}*/
		//From Frank's test
    UARTprintf("Servo Test Starting...\n");
    for(;;){
        static int i = 0;
        SetServoOne(i);
        UARTprintf("Servo Output: %d   \r",i);
        i = (i+1)%256;
        WaitUS(100000);
	}
}
