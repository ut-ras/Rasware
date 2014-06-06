// Handlers for pub/sub
// Kevin Gilbert
// November 30 2013
#include <RASLib/inc/json_protocol.h>
#include <RASLib/inc/rasstring.h>       // SPrintf
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/encoder.h>

#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_types.h>
#include <StellarisWare/inc/hw_watchdog.h>
#include <StellarisWare/driverlib/watchdog.h>

#include "handlers.h"
#include "pid.h"

#include "atospf.h"

/**
*	Message Buffers for subscribers/publishers
*/
char msgBuffSPLM[BUFFERSIZE] = {'-'};
char msgBuffSPRM[BUFFERSIZE] = {'-'};
char msgBuffSVLX[BUFFERSIZE] = {'-'};
char msgBuffSVAX[BUFFERSIZE] = {'-'};
char msgBuffRSTE[BUFFERSIZE] = {'-'};
char* msgBuffer[NUMSUB+NUMPUB] = {msgBuffSPLM, msgBuffSPRM, msgBuffSVLX, msgBuffSVAX, msgBuffRSTE};

/**
*   Velocity control variables
*/
signed long goalDeltaTicks = 1000;
    
float prevCommandLeft = 0;
float prevCommandRight = 0;
signed long prevTicksLeft = 0;
signed long prevTicksRight = 0;

extern tMotor *left_motor, *right_motor;
extern tEncoder *left_encoder, *right_encoder;
extern tPID pidLeft, pidRight;

float parseSpeed(char* jsonvalue) 
{
    float speed = atospf(jsonvalue);
    
    if (speed < 0) {
        if(speed < -MAXPWM) 
            speed = -MAXPWM;
    } else {
        if(speed > MAXPWM) 
            speed = MAXPWM;
    }

	return speed;
}



/**
*	Subscriber Handlers
*   
*/								   
void SPLM_handler(void* data, char *jsonvalue) {
    float speed = parseSpeed(jsonvalue);

    SPrintf(msgBuffSPLM, "%f",speed);

    SetPin(PIN_F1, false);
    WatchdogReloadSet(WATCHDOG_BASE, 25000000);    
    SetMotor(left_motor, speed);
}

void SPRM_handler(void* data, char *jsonvalue) {
    float speed = parseSpeed(jsonvalue);

	//Printf("speed %s -> %f\n", jsonvalue, speed);

    SPrintf(msgBuffSPRM, "%f",speed);

    SetPin(PIN_F1, false);
    WatchdogReloadSet(WATCHDOG_BASE, 25000000);    
    SetMotor(right_motor, speed);
}

void SVLX_handler(void* data, char *jsonvalue) {
    int val;
    float speed;

    signed long ticksLeft = GetEncoder(left_encoder);
    signed long ticksRight = GetEncoder(right_encoder);
    signed long deltaTicksLeft = ticksLeft - prevTicksLeft;
    signed long deltaTicksRight = ticksRight - prevTicksRight;
        
    float motorCommandLeft = prevCommandLeft + RunPID(&pidLeft, goalDeltaTicks, deltaTicksLeft);
    float motorCommandRight = prevCommandRight + RunPID(&pidRight, goalDeltaTicks, deltaTicksRight);

    if(motorCommandLeft > MAXMOTOR)
        motorCommandLeft = MAXMOTOR;
    if(motorCommandLeft < MINMOTOR)
        motorCommandLeft = MINMOTOR;
    if(motorCommandRight > MAXMOTOR)
        motorCommandRight = MAXMOTOR;
    if(motorCommandRight < MINMOTOR) 
        motorCommandRight = MINMOTOR;

    WatchdogReloadSet(WATCHDOG_BASE, 25000000); 
    SetPin(PIN_F1, false);

    SetMotor(left_motor, motorCommandLeft);
    SetMotor(right_motor, motorCommandRight);
        
    prevTicksLeft = ticksLeft;
    prevTicksRight = ticksRight;
    prevCommandLeft = motorCommandLeft;
    prevCommandRight = motorCommandRight;
    SPrintf(msgBuffSVLX, "L:%06d R:%06d", deltaTicksLeft, deltaTicksRight);
}

void SVAX_handler(void* data, char *jsonvalue) {
    SPrintf(msgBuffSVAX, "%s", jsonvalue);    
}

void RSTE_handler(void* data, char *jsonvalue) {
    SPrintf(msgBuffRSTE, "%s", jsonvalue);
}

void (*subHandlers[NUMSUB])(void*,char*) = {SPLM_handler, SPRM_handler,
			                    SVLX_handler, SVAX_handler, RSTE_handler};

/**
* 	Publisher Handlers
*/

char* SPLMdebug_handler(void* data) {
    return msgBuffSPLM;
}

char* SPRMdebug_handler(void* data) {
    return msgBuffSPRM;
}

char* SVLXdebug_handler(void* data) {
    return msgBuffSVLX;
}

char* SVAXdebug_handler(void* data) {
    return msgBuffSVAX;
}

char* (*pubHandlers[NUMPUB])(void*) = {SPLMdebug_handler, SPRMdebug_handler,
			               SVLXdebug_handler, SVAXdebug_handler};
									           
