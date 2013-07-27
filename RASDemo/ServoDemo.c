#include <utils/uartstdio.h>	// input/output over UART
#include <RASLib/inc/servo.h>
#include "RASDemo.h"
#include "time.h"
#include "servo.h"

tServo *servo;

void initServo(void) {
    servo = InitializeServo(PIN_A2);
}

void servoDemo(void) {
	// 2012 Servo Demo code
	UARTprintf("Press:\n  a-'up' 0.10\n  w-'up' 0.01\n  s-'down' 0.01\n");
	UARTprintf("  d-'down' 0.10\n  enter-quit\n");
	
	{   float position = 0;
		char newline = 13;
		char ch;
		
		UARTprintf("position: 0x%x%x  ", position);
		ch = getc();
        
		while(ch != newline) {
			if (ch == 'w')
				position += 0.01f;
			else if (ch == 's')
				position -= 0.01f;	 
			else if (ch == 'a')
				position += 0.10f;
			else if (ch == 'd')
				position -= 0.10f;
            
			SetServo(servo, position);
            
			UARTprintf("\rposition: 0x%x%x  ",position);	 
			ch = getc();
		}
				 
		UARTprintf("\n");
	}
}
