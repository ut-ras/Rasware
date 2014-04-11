#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control

tMotor *motors[2];

void PortD_Init(){
	int delay = SYSCTL_RCGC2_R;      // allow time for clock to stabilize
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	
  GPIO_PORTD_DIR_R |= 0x01;    // make PD0 Output
  GPIO_PORTD_AFSEL_R &= ~0x01; // regular port function 
  GPIO_PORTD_DEN_R |= 0x01;
}

void initMotors(void) {
    motors[0] = InitializeServoMotor(PIN_D0, false);
   // motors[1] = InitializeTLEMotor(PIN_C5, PIN_C4, true, false);
    //motors[0] = InitializeTLEMotor(PIN_F1, PIN_F0, true, false);
    //motors[1] = InitializeTLEMotor(PIN_F3, PIN_F2, true, false);

    SetMotor(motors[0], 0);
   // SetMotor(motors[1], 0);
}

void motorDemo(void) {
    Printf("Press:\n  w-forward\n  s-backward\n  a-left\n  ");
    Printf("d-right\n  space-stop\n  enter-quit\n");
  
    {
        float left = 0, right = 0, speed = 0.075f, accel = 0.01f;
        char newline = 13;
        char ch = Getc();
        while(ch != newline) {
            ch = Getc();
            Printf("%c", ch);
            switch(ch) {
                case 'w':
                    left = speed;
                    right = speed;
                    break;
                case 's':
                    left = -speed;
                    right = -speed;
                    break;
                case 'a':
                    left = -speed;
                    right = speed;
                    break;
                case 'd':
                    left = speed;
                    right = -speed;
                    break;
                case 'i':
                    right += accel;
                    left += accel;
                    break;
                case 'k':
                    right -= accel;
                    left -= accel;
                    break;
                case 'j':
                    right -= accel;
                    left += accel;
                    break;
                case 'l':
                    right += accel;
                    left -= accel;
                    break;
                default:
                    left = 0; 
                    right = 0;
                    break;
            }

            SetMotor(motors[0], left);
           // SetMotor(motors[1], right);
            Printf(" Set Motor to %d %d  \r", (int)(left*100), (int)(right*100));
        }
    }                 
    
    SetMotor(motors[0], 0.0f);
    //SetMotor(motors[1], 0.0f);
    Printf("\n");
}
