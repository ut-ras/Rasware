#include <inc/hw_types.h>		// tBoolean
#include "RASDemo.h"
#include <RASLib/inc/uart.h>
#include <RASLib/inc/linesensor.h> 
#include <RASLib/inc/init.h> 
#include <RASLib/inc/time.h>

int main(void)
{	
	char ch;	  	 

	InitializeMCU();
	InitializeUART();

	while(1) {
		Printf("\nRAS Demo for Robotathon 2013\n");
		Printf("  0=UART Demo\n  1=Motor Demo\n");
		Printf("  2=Servo Demo\n  3=Line Sensor\n");
		Printf("  4=IR Sensor Demo\n  5=Encoders Demo\n");
		
		Printf(">> ");
		ch = Getc();
		Printf("%c", ch);
		Printf("\n");

		if (ch == '0') {
			Printf("\n UART Demo\n");
			uartDemo();	 
		}
		else if (ch == '1') {
			Printf("\nMotor Demo\n");
			initMotors();
			motorDemo(); 
		}
		else if (ch == '2') {
			Printf("\nServo Demo\n");
			initServo();
			servoDemo();   
		}
		else if (ch == '3') {			   
			Printf("\nLine Sensor Demo\n");
			InitializeLineSensor();		  
			lineSensorDemo();
		}
		else if (ch == '4') {	   
			Printf("\nIR Sensor Demo\n");
			initIRSensor();
			IRSensorDemo();	 
		}
		else if (ch == '5') {
			Printf("\nEncoders Demo\n");
			initEncoders();
			encoderDemo();
		}
	}
}
