#include "vel_control.h"

tVCAction VCRun(
    tVC *vc, 
    tVels *desired, 
    signed long deltaRightTicks, 
    signed long deltaLeftTicks,
    float timeStep // seconds
    )
{
    float ticksPerLinearVel = timeStep / vc->r->ticksPerUnit,
          ticksPerAngularVel = timeStep / (vc->r->ticksPerUnit/vc->r->unitsAxisWidth);
    
    // convert angular and linear velocities into encoder ticks
    float desiredLinearTicks = desired->v * ticksPerLinearVel,
          desiredAngularTicks = desired->w * ticksPerAngularVel;

    // convert desired and current into left and right encoder ticks
    float desiredLeftTicks = desiredLinearTicks + desiredAngularTicks,
          desiredRightTicks = desiredLinearTicks - desiredAngularTicks;

    // run left and right PID loops to decide left and right motor powers 
    tVCAction output;
    output.rightMotor = RunPID(vc->right, desiredRight, deltaRightTicks);
    output.leftMotor = RunPID(vc->left, desiredLeft, deltaLeftTicks); 

    return output;
}

InitializeVC(
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
}

