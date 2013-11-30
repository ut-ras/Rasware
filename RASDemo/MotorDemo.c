#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>

tMotor *leftMotor;
tMotor *rightMotor;
tBoolean initialized = false;

void initMotors(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    leftMotor = InitializeMotor(PIN_B7, PIN_B6, true, false);
    rightMotor = InitializeMotor(PIN_C5, PIN_C4, true, false);
    //leftMotor = InitializeMotor(PIN_F1, PIN_F0, true, false);
    //rightMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);

    SetMotor(leftMotor, 0);
    SetMotor(rightMotor, 0);
}

void motorDemo(void) {
    float left = 0, right = 0, speed = 0.75f, accel = 0.01f;
    char newline = 13;
    char ch;    

    Printf("Press:\n  w-forward\n  s-backward\n  a-left\n  d-right\n");  
    Printf("  i-slowly forward\n  k-backward\n  j-left\n  l-right\n");  
    Printf("  space-stop\n  enter-quit\n");
  
    ch = Getc();
    
    while (ch != newline) {
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
        
        // bounds checking (done in SetMotor, but also useful to bound it for the demo)
        if (left > 1.0f) {
             left = 1.0f;
        } else if (left < -1.0f) {
             left = -1.0f;
        }

        if (right > 1.0f) {
             right = 1.0f;
        } else if (right < -1.0f) {
             right = -1.0f;
        }

        SetMotor(leftMotor, left);
        SetMotor(rightMotor, right);
        Printf("%c  set motor to %1.2f %1.2f\r", ch, left, right);
        
        ch = Getc();
    }                 
    
    SetMotor(leftMotor, 0.0f);
    SetMotor(rightMotor, 0.0f);
    Printf("\n");
}
