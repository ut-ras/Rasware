#include <StellarisWare/inc/hw_types.h>		// tBoolean
#include "RASDemo.h"
#include <StellarisWare/utils/uartstdio.h>	// input/output over UART
#include <RASLib/inc/uart.h> 
#include <RASLib/inc/init.h> 

int main(void)
{	
	char ch;	  	 

	InitializeMCU();
	initUART();

	while(1) {	
		UARTprintf("\nRAS Demo for Robotathon 2012\n");
		UARTprintf("  0=UART Demo\n  1=Motor Demo\n");
		UARTprintf("  2=Servo Demo\n  3=Line Sensor\n");
		UARTprintf("  4=IR Sensor Demo\n  5=Encoders Demo\n");

		UARTprintf(">> ");
		ch = getc();
		putc(ch);
		UARTprintf("\n");

		if (ch == '0') {
			UARTprintf("\nUART Demo\n");
			uartDemo();	 
		}
		else if (ch == '1') {
			UARTprintf("\nMotor Demo\n");
			UARTprintf("\n :P \n"); 
			//initMotors();
			//motorDemo(); 
		}
		else if (ch == '2') {
			UARTprintf("\nServo Demo\n");
			UARTprintf("\n :P \n");
			//initServo();
			//servoDemo();   
		}
		else if (ch == '3') {			   
			UARTprintf("\nLine Sensor Demo\n");
			UARTprintf("\n :P \n");
			//initLineSensor();		  
			//lineSensorDemo();
		}
		else if (ch == '4') {	   
			UARTprintf("\nIR Sensor Demo\n");
			UARTprintf("\n :P \n");		
			//initIRSensor();
			//IRSensorDemo();	 
		}
		else if (ch == '5') {
			UARTprintf("\nEncoders Demo\n");
			UARTprintf("\n :P \n");
			//initEncoders();
			//encoderDemo();
		}
	}
}
