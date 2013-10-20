#include <RASLib/inc/motor.h>

#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/pwm.h>
#include <RASLib/inc/common.h>
tMotor *motors[2];

void initMotors(void) {
    motors[0] = InitializeMotor(PIN_B7, PIN_B6, true, true);
    motors[1] = InitializeMotor(PIN_C5, PIN_C4, true, true);
}
// The 'main' function is the entry point of the program
int main(void) {
	 float left = 0, right = 0, speed = 0.2;
    InitializeMCU();
	initMotors();
   
    // Initialization code can go here
    SetMotor(motors[0], speed);
    SetMotor(motors[1], speed);
    while (1) {
         
         // Runtime code can go here
         
     }
}
