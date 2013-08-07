#include <inc/hw_types.h>		// tBoolean
#include "RASDemo.h"
#include <utils/uartstdio.h>	// input/output over UART
#include <RASLib/inc/uart.h> 
#include <RASLib/inc/init.h> 

#include <RASLib/inc/time.h>

#include <RASLib/inc/sonar.h>

int main(void)
{	
	char ch;	  	 

	InitializeMCU();
	initUART();

	while(1) {
		UARTprintf("\nRAS Demo for Robotathon 2013\n");
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
			initMotors();
			motorDemo(); 
		}
		else if (ch == '2') {
			UARTprintf("\nServo Demo\n");
			initServo();
			servoDemo();   
		}
		else if (ch == '3') {			   
			UARTprintf("\nLine Sensor Demo\n");
			initLineSensor();		  
			lineSensorDemo();
		}
		else if (ch == '4') {	   
			UARTprintf("\nIR Sensor Demo\n");
			initIRSensor();
			IRSensorDemo();	 
		}
		else if (ch == '5') {
			UARTprintf("\nEncoders Demo\n");
			initEncoders();
			encoderDemo();
		}
	}
}
