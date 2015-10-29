#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>

static tMotor *Motors[4];
static tBoolean initialized = false;

void initMotors(void) {
    if (!initialized) {
      initialized = true;
      
      Motors[0] = InitializeServoMotor(PIN_B6, false);
      Motors[1] = InitializeServoMotor(PIN_B7, false);
      Motors[2] = InitializeServoMotor(PIN_C4, false);
      Motors[3] = InitializeServoMotor(PIN_C5, false);
    }
}


void motorDemo(void) {
    float left = 0, right = 0, speed = 0.75f, accel = 0.01f;
    char ch;    
    int i;

    Printf("Press:\n"
	   "  w-forward\n"
	   "  s-backward\n"
	   "  a-left\n"
	   "  d-right\n"    
           "  i-slowly forward\n"
	   "  k-slowly backward\n"
	   "  j-slowly left\n"
	   "  l-slowly right\n"    
           "  space-stop\n"
	   "  enter-quit\n");
  
    // wait for the user to enter a character
    ch = ' ';
    
    while (ch != '\n') {
        switch (ch) {
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

        SetMotor(Motors[0], left);
        SetMotor(Motors[1], left);
        SetMotor(Motors[2], right);
        SetMotor(Motors[3], right);
        Printf(" set motor to %1.2f %1.2f  \r", left, right);
        
        ch = Getc();
    } 
   
    // make sure the motors are off before exiting the demo 
    for (i = 0; i < 4; ++i) 
      SetMotor(Motors[i], 0);
    Printf("\n");
}
