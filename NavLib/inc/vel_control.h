#include "common.h"

#ifndef __VEL_CONTROL_H__
#define __VEL_CONTROL_H__

typedef struct {
    tPID right;
    tPID left;
    
    // are these needed too?
    float unitsAxisWidth; 
    float ticksPerUnit; 
} tVC;

typedef struct {
    float rightMotorPower, leftMotorPower;
} tVCAction;

tVCAction VCRun(tVC *vc, tVels *desired, tVels *current);

InitializeVC(tVC *vc, float p, float i, float d);

#endif // __VEL_CONTROL_H__
