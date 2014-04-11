#include <inc/hw_types.h>
#include <StellarisWare/driverlib/gpio.h>
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/uart.h>

tBoolean led_on;
tMotor *Right_Wheel;		
tMotor *Arm;
tMotor *Left_Wheel;

#define p0 PIN_D0
#define p1 PIN_D1
#define p2 PIN_D2
#define p3 PIN_D3

tServo *Claw;
tServo *Cam_Ver;
tServo *Cam_Hor;

#define c4 PIN_C4
#define c5 PIN_C5
#define c6 PIN_C6
#define c7 PIN_C7

tADC *a0;
tADC *a1;

float cam_ver_pos;
float cam_hor_pos;
float left_wheel_pos;
float right_wheel_pos;
float arm_pos;
float claw_pos;


void all_init (void)
{
	cam_ver_pos = 50.0f;
	cam_hor_pos = 50.0f;
	left_wheel_pos = 0.0f;
	right_wheel_pos = 0.0f;
	arm_pos = 0.0f;
	claw_pos = 0.0f;
	
	Claw = InitializeServo(PIN_C7);
	Cam_Ver = InitializeServo(PIN_C5);
	Cam_Hor = InitializeServo(PIN_C6);
	
	Right_Wheel = InitializeServoMotor(PIN_D1, 0);
  Left_Wheel = InitializeServoMotor(PIN_D3, 0);
	Arm = InitializeServoMotor(PIN_D2, 0);
}


void blink(void)
{
    SetPin(PIN_F1, led_on);
    led_on = !led_on;
}
//SERVOS 

void setCam_vertical(int dir){ //Takes input from Selma's code and decides whether to move the camera up and down
		if ( dir==1){	//up
			cam_ver_pos+=.01f;
		}
		
		else{ //down 
			
			cam_ver_pos-=.01f;
		}
		
		 if(cam_ver_pos > 1.00f) {  //Bounds checking
					cam_ver_pos= 1.00f;
					
        } else if(cam_ver_pos < 0.00f) {
             cam_ver_pos = 0.00f;
        }
				
				
		SetServo(Cam_Ver,cam_ver_pos);
}

void setCam_horizontal (int dir){
	if (dir==1){ //left
		cam_hor_pos+=0.01f;
	}
	
	else  //right
		cam_hor_pos-=0.01f;
	
	
		 if(cam_hor_pos > 1.00f) {  //Bounds checking
					cam_hor_pos = 1.00f;
					
        } else if(cam_hor_pos < 0.00f) {
             cam_hor_pos = 0.00f;
        }
				
	
	SetServo(Cam_Hor, cam_hor_pos);
}

void setClaw (int dir){
	if (dir ==1){ //open
		claw_pos-=0.05f;
	}
	
	else //close
		claw_pos+=0.05f;
	
	
	 if(claw_pos > 1.00f) {  //Bounds checking
					claw_pos= 1.00f;
					
        } else if(claw_pos < 0.00f) {
             claw_pos = 0.00f;
        }
	
	SetServo(Claw, claw_pos);
}
	
// MOTORS
void setWheel_Left (int dir){
  if (dir==1){ //forward
		left_wheel_pos+= 0.075f;
	}
	
	else //backward
		left_wheel_pos-= 0.075f;
	
	SetMotor(Left_Wheel, left_wheel_pos);
}

void setWheel_Right (int dir){
  if (dir==1){ //forward
		right_wheel_pos-= 0.075f;
	}
	
	else //backward
		right_wheel_pos+= 0.075f;
	
	SetMotor(Right_Wheel, right_wheel_pos);
}

void setArm (int dir) {
	if (dir==1){ //up
		 arm_pos+= 0.2f;
	}
	
	else //down
		arm_pos-= 0.2f;
	
	SetMotor(Arm, arm_pos);
	
}
int main(void) {  
    char ch;
    InitializeMCU();
	InitializeUART();
	all_init();
	
    CallEvery(blink, 0, 0.25f);
		Printf("Mercury Bot Starter Code \n");
        Printf("wasd for moving \n");
			  Printf("[] for open and closing claw \n");
			  Printf(",. for arm up and down \n");
			  Printf("arrow keys for camera \n");
        Printf(">> ");
	
    while(1) {
        // Read input from User
        ch = Getc();	//<== to use this u need the uart.h
			
        switch(ch) {
					  //Movement of bot
            case 'w':
                Printf("\n Moving forward \n");
                setWheel_Left(1);
                setWheel_Right(1);						
                break;
            case 's':
                Printf("\n Moving backward \n");
                setWheel_Left(-1);
								setWheel_Right(-1);
                break;
            case 'a':
                Printf("\n Moving left \n");
                setWheel_Left(-1);
								setWheel_Right(1);
                break;
            case 'd':
                Printf("\n Moving right \n");
                setWheel_Left(1);
								setWheel_Right(-1); 
                break;
						
						//Operation of claw
            case '[':                            
                Printf("\n Opening claw \n");
                setClaw(1);  
                break;
            case ']':
                Printf("\n Closing claw\n");
                setClaw(-1);
                break;
						
						//Operation of arm
            case ',':
                Printf("\n Raising arm \n");
                setArm(1);
                break;
            case '.':
                Printf("\n Lowering arm \n");
                setArm(-1);
                break;
						
						//Operation of camera
				case 'j':
                Printf("\n Cam left \n");
                setCam_horizontal(1);
                break;
				case 'i':
                Printf("\n Raising cam \n");
                setCam_vertical(1);
                break;
				case 'k':
                Printf("\n Lowering cam \n");
                setCam_vertical(-1);
                break;
				case 'l':
                Printf("\n Cam right \n");
                setCam_horizontal(-1);
                break;						
        }				
    }
}
