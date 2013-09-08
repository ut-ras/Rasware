#include "RASDemo.h"

#include <RASLib/inc/uart.h>
#include <RASLib/inc/servo.h>

tServo *servo;

void initServo(void) {
    servo = InitializeServo(PIN_B0);
}

void servoDemo(void) {
	// 2012 Servo Demo code
	Printf("Press:\n  a-'up' 0.10\n  w-'up' 0.01\n  s-'down' 0.01\n");
	Printf("  d-'down' 0.10\n  enter-quit\n");
	
	{   float position = 0;
		char newline = 13;
		char ch;
		
		Printf("position: %f ", position);
		ch = Getc();
        
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
            
			Printf("\rposition: %f ",position);	 
			ch = Getc();
		}
				 
		Printf("\n");
	}
}
