#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>

tBoolean led_on;

void blink(void) {
    SetPin(PIN_F1, led_on);

    led_on = !led_on;
}

int main(void) {  
    char ch;       
    InitializeMCU();
    CallEvery(blink, 0, 0.25f);

    while(1) {
        Printf("\nRAS Demo for Robotathon 2013\n");
        Printf("  0=UART Demo\n  1=Motor Demo\n");
        Printf("  2=Servo Demo\n  3=Line Sensor\n");
        Printf("  4=IR Sensor Demo\n  5=Encoders Demo\n");
        Printf("  6=GPIO Demo\n");
        
        Printf(">> ");
        // Read input from User
        ch = Getc();
        Printf("%c", ch);
        Printf("\n");

        switch(ch) {
            case '0':
                Printf("\n UART Demo\n");
                uartDemo();                 
                break;
            case '1':
                Printf("\nMotor Demo\n");
                initMotors();
                motorDemo(); 
                break;
            case '2':
                Printf("\nServo Demo\n");
                initServo();
                servoDemo();     
                break;
            case '3':
                Printf("\nLine Sensor Demo\n");
                initLineSensor();          
                lineSensorDemo();    
                break;
            case '4':                            
                   Printf("\nIR Sensor Demo\n");
                initIRSensor();
                IRSensorDemo();     
                break;
            case '5':
                Printf("\nEncoders Demo\n");
                initEncoders();
                encoderDemo();
                break;
            case '6':
                Printf("\nGPIO Demo\n");
                gpioDemo();
                break;
        }
    }
}
