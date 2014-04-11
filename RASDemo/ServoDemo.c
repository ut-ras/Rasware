#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/servo.h>
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control

tServo *servo;

void initD(void)
{
	int delay = SYSCTL_RCGC2_R;      // allow time for clock to stabilize
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	
  GPIO_PORTD_DIR_R |= 0x01;    // make PD0 Output
  GPIO_PORTD_AFSEL_R &= ~0x01; // regular port function 
  GPIO_PORTD_DEN_R |= 0x01;
}

void initServo(void) {
   // servo = InitializeServo(PIN_B0);
	
	servo= InitializeServo(PIN_D0);
}



void servoDemo(void) {
    /**************************************************
    * Servo takes position argument 0.00f-1.00f
    *
    **************************************************/
    float position = 0;
    char newline = 13;
    char ch;

    Printf("Press:\n  a-'up' 0.10\n  w-'up' 0.01\n  s-'down' 0.01\n");
    Printf("  d-'down' 0.10\n  enter-quit\n");

    Printf("position: %f ", position);
    ch = Getc();
        
       while(ch != newline) {
        switch(ch) {
            case 'w':
                position += 0.1f;
                break;
            case 's':
                position -= 0.1f;
                break;
            case 'a':
                position += 0.50f;
                break;
            case 'd':
                position -= 0.50f;
                break;
            default:
                position = position;
        }
        // Update from 2012, includes bounds checking (done with SetServo,
        //   but makes user function easier to decrease.
        if(position > 1.00f) {
					position= 1.00f;
					
        } else if(position < 0.00f) {
             position = 0.00f;
        }

        SetServo(servo, position);
           
        Printf("\rposition: %f ",position);     
        ch = Getc();
    }         
    Printf("\n");		//michelle is lame 
}
