#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/servo.h>

tServo *servo;

void initServo(void) {
    servo = InitializeServo(PIN_B0);
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
        // Update from 2012, includes bounds checking (done with SetServo,
        //   but makes user function easier to decrease.
        if(position > 1.00f) {
             position = 1.00f;
        } else if(position < 0.00f) {
             position = 0.00f;
        }

        SetServo(servo, position);
           
        Printf("\rposition: %f ",position);     
        ch = Getc();
    }         
    Printf("\n");
}
