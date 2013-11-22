#include "vel_control.h"

static signed long diffTicks(signed long cur, signed long old) {
    // TODO: check and correct for over/underflow
    return cur - old;
}

tVCAction RunVC(
    tVC *vc, 
    tVels *desired, 
    signed long leftTicks,
    signed long rightTicks, 
    float timeStep // seconds
    )
{
    // calculate conversion constants (maybe timeStep should be in the vc struct?)
    float ticksPerLinearVel = timeStep / vc->r->ticksPerUnit,
          ticksPerAngularVel = timeStep / (vc->r->ticksPerUnit/vc->r->unitsAxisWidth);
    
    // convert angular and linear velocities into encoder ticks
    float desiredLinearTicks = desired->v * ticksPerLinearVel,
          desiredAngularTicks = desired->w * ticksPerAngularVel;

    // convert desired and current into left and right encoder ticks
    float desiredLeftTicks = desiredLinearTicks + desiredAngularTicks,
          desiredRightTicks = desiredLinearTicks - desiredAngularTicks;

    float deltaLeftTicks = (float)diffTicks(leftTicks, vc->prevLeftTicks),
          deltaRightTicks = (float)diffTicks(rightTicks, vc->prevRightTicks);

    // run left and right PID loops to decide left and right motor powers 
    tVCAction output;
    output.rightMotor = RunPID(&(vc->right), desiredRightTicks, deltaRightTicks);
    output.leftMotor = RunPID(&(vc->left), desiredLeftTicks, deltaLeftTicks); 

    return output;
}

void InitializeVC(
    tVC *vc, 
    tRobot *r, 
    float p, 
    float i, 
    float d, 
    float min, 
    float max
    )
{
    vc->r = r;
    InitializePID(&(vc->right), p, i, d, min, max);
    InitializePID(&(vc->left), p, i, d, min, max);
    vc->prevLeftTicks = 0;
    vc->prevRightTicks = 0;
}

