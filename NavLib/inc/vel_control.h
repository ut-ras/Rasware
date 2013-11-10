#include "common.h"

#ifndef __VEL_CONTROL_H__
#define __VEL_CONTROL_H__

typedef struct {
    tPID right;
    tPID left;
    tRobot *r;
} tVC;

typedef struct {
    float rightMotor;
    float leftMotor;
} tVCAction;

tVCAction VCRun(
    tVC *vc, 
    tVels *desired, 
    signed long deltaRightTicks, 
    signed long deltaLeftTicks,
    float timeStep // seconds
    );

InitializeVC(
    tVC *vc, 
    tRobot *r, 
    float p, 
    float i, 
    float d, 
    float min, 
    float max
    );

#endif // __VEL_CONTROL_H__

