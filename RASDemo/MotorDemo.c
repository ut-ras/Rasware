#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>

tMotor *motors[12];

void initMotors(void) {
    motors[0] = InitializeMotor(PIN_B6, PIN_B7, true, false);
    motors[1] = InitializeMotor(PIN_E5, PIN_E4, true, false);
}

void motorDemo(void) {
  Printf("Press:\n  w-forward\n  s-backward\n  a-left\n  ");
  Printf("d-right\n  space-stop\n  enter-quit\n");
  
  {
    float left = 0, right = 0, speed = 0.75;
    char newline = 13;
    char ch = Getc();
    while(ch != newline) {
      ch = Getc();
      Printf("%c", ch);
      if (ch == 'w') {
        left = speed;
        right = speed;
      } else if (ch == 's') {
        left = -speed;
        right = -speed;
      } else if (ch == 'a') {
        left = -speed;
        right = speed;
      } else if (ch == 'd') {
        left = speed;
        right = -speed;
      } else if (ch == ' ') {
        left = 0;
        right = 0;
      }
      SetMotor(motors[0], left);
      SetMotor(motors[1], right);
      Printf(" Set Motor to %d %d  \r", (int)(left*100), (int)(right*100));
    }
  }
  
  SetMotor(motors[0], 0.0f);
  SetMotor(motors[1], 0.0f);
  Printf("\n");
}
