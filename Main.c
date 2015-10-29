#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/pwm.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>
//#include <RASLib/inc/pid.h>
/*
#define Front 0
#define Left 1
#define Right 2

//Intializations
static tMotor *Motors[3];
static tADC *adc[2];
tBoolean blink_on = true;
tBoolean initialized = false; */
tServo *servoTest;
/*
//variables for PID control
float k_p = 4;
float k_i = 0;
float k_d = 0;

void initRobot(void){
    if (!initialized){
        initialized = true;

        //Intializing Motors
          Motors[Front] = IntializeServoMotor(PIN_B5, false);
          Motors[Left] = IntializeServoMotor(PIN_B0, true);
          Motors[Right] = InitializeServoMotor(PIN_B1, false);

       //Intializing Sensors
          adc[Front] = InitializeADC(PIN_D0);
          adc[Left] = InitializeADC(PIN_D1);
     }

}
    
// The 'main' function is the entry point of the program
int main(void) {
  float wallDistance = 0;
  float frontSensor = 0;
  float error = 0;
  float setpoint = .5f
  float errorSum = 0;
  float errorPrev = 0;
  float errorDeriv = 0;
  
    initRobot();
    
   // servoTest = InitializeServoMotor(PIN_B2,false); */
    servoTest = InitializeServo(PIN_B0);
   // CallEvery(blink, 0, 0.25);
   /* SetMotor(Motors[Left], 0.0f);
      SetMotor(Motors[Right], 0.0f);
      SetMotor(Motors[Front], 0.0f);
*/
    while (1) {
        SetServo(servo, 1.0f);
      /*  frontSensor = ADCRead(adc[Front]);
        error = setpoint - frontSensor;
        errorDeriv = error - errorPrev;
        wallDistatnce = k_p*error + k_i*errorSum + k_d*errorDeriv;
        SetMotor(Motors[Left], 1 - wallDistance);   
        SetMotor(Motors[Right], 1 + wallDistance);
        //SetMotor(Motors[Front], wallDistance);
        errorPrev = error;
        
        //Printf("Hello World!\n");
       */
        
        
    }
}
