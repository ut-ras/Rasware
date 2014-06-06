#include "vel_control.h"
#include "extra_math.h"

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
    tVCAction output;
    float leftUnits, rightUnits, leftUnitsDesired, rightUnitsDesired;
    
    // calculate how far each wheel has travelled
    leftUnits = vc->r->ticksPerUnit * (float)diffTicks(leftTicks, vc->prevLeftTicks),
    rightUnits = vc->r->ticksPerUnit * (float)diffTicks(rightTicks, vc->prevRightTicks);
    
    // calculat how far we wanted each wheel to travel
    if (fequals(desired->w, 0.0f)) {
        leftUnitsDesired = desired->v * timeStep;
        rightUnitsDesired = desired->v * timeStep;
    } else {
        leftUnitsDesired = (desired->v - desired->w * vc->r->unitsAxisWidth / 2.0f) * timeStep;
        rightUnitsDesired = (desired->v + desired->w * vc->r->unitsAxisWidth / 2.0f) * timeStep;
    }

    // run left and right PID loops to decide left and right motor speeds
    output.leftMotor = vc->prevOutput.leftMotor + RunPID(&(vc->left), leftUnitsDesired, leftUnits);
    output.rightMotor = vc->prevOutput.rightMotor + RunPID(&(vc->right), rightUnitsDesired, rightUnits);
    
    vc->prevLeftTicks = leftTicks;
    vc->prevRightTicks = rightTicks;
    vc->prevOutput = output;

    return output;
}

void InitializeVC(
    tVC *vc, 
    tRobotData *r, 
    float leftP, 
    float leftI, 
    float leftD, 
    float rightP, 
    float rightI, 
    float rightD, 
    float min, 
    float max
    )
{
    vc->r = r;
    InitializePID(&(vc->left), leftP, leftI, leftD, min, max);
    InitializePID(&(vc->right), rightP, rightI, rightD, min, max);
    vc->prevLeftTicks = 0;
    vc->prevRightTicks = 0;
    vc->prevOutput.leftMotor = 0;
    vc->prevOutput.rightMotor = 0;
}

