#include "common.h"

#ifndef __VEL_CONTROL_H__
#define __VEL_CONTROL_H__

typedef struct {
    tPID right;
    tPID left;
    
    float ticksPerLinearVel;
    float ticksPerAngularVel; 
} tVC;

typedef struct {
    float rightMotor, leftMotor;
} tVCAction;

tVCAction VCRun(tVC *vc, tVels *desired, signed long deltaRightTicks, signed long deltaLeftTicks);

InitializeVC(tVC *vc, float p, float i, float d, float min, float max);

#endif // __VEL_CONTROL_H__

