#ifndef __VEL_CONTROL_H__
#define __VEL_CONTROL_H__

struct {
    float p, i d;
    float prevErr, accumErr;
} typedef tPID;

struct {
    tPID right, left;
    float outputRight, outputLeft;
} typedef tVelControl;

void VelControlIterate(tVelControl *vc, float inputRight, float inputLeft);

InitializeVelControl(tVelControl *vc, float p, float i, float d);

#endif // __VEL_CONTROL_H__
