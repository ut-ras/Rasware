#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/servo.h>

tServo *servo;
tBoolean initialized = false;

void initServo(void) {
    // don't initialize this if we've already done so
    if (initialized) {
        return;
    }
    
    initialized = true;

    servo = InitializeServo(PIN_B0);
}

void servoDemo(void) {
    float position = 0;
    char newline = 13;
    char ch;

    Printf("Press:\n  a-'up' 0.10\n  w-'up' 0.01\n  s-'down' 0.01\n");
    Printf("  d-'down' 0.10\n  enter-quit\n");
    
    // wait for the user to enter a character
    ch = Getc();
        
    while (ch != newline) {
        switch (ch) {
            case 'w':
                position += 0.01f;
                break;
            case 's':
                position -= 0.01f;
                break;
            case 'a':
                position += 0.10f;
                break;
            case 'd':
                position -= 0.10f;
                break;
            default:
                position = position;
        }
        
        // bounds checking (done in SetServo, but also useful to bound it here for the demo)
        if (position > 1.0f) {
             position = 1.0f;
        } else if (position < 0.0f) {
             position = 0.0f;
        }

        SetServo(servo, position);
        Printf("%c  set servo to %1.2f\r",position);
        
        ch = Getc();
    }         
    
    Printf("\n");
}

