#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/pwm.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/pid.h>

// Blink the LED to show we're on
tBoolean blink_on = true;
tServo *servoTest;

void blink(void) {
SetPin(PIN_F3, blink_on);
blink_on = !blink_on;   
}


// The 'main' function is the entry point of the program
int main(void) {
  
    
   // servoTest = InitializeServoMotor(PIN_B2,false);
    servoTest = InitializeServo(PIN_B0);
    CallEvery(blink, 0, 0.25);
    
    while (1) {
        SetServo(servoTest, 1.0f);
        //Printf("Hello World!\n");
        [Front]
        [Back]
        
    }
}
