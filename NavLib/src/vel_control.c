#include "vel_control.h"

tVCAction VCRun(tVC *vc, tVels *desired, signed long deltaRightTicks, signed long deltaLeftTicks) {
    // convert angular and linear velocities into encoder ticks
    float desiredLinearTicks = desired->v * vc->ticksPerLinearVel,
          desiredAngularTicks = desired->w * vc->ticksPerAngularVel;

    // convert desired and current into left and right encoder ticks
    float desiredLeftTicks = desiredLinearTicks + desiredAngularTicks,
          desiredRightTicks = desiredLinearTicks - desiredAngularTicks;

    // run left and right PID loops to decide left and right motor powers 
    tVCAction output;
    output.rightMotor = RunPID(vc->right, desiredRight, deltaRightTicks);
    output.leftMotor = RunPID(vc->left, desiredLeft, deltaLeftTicks); 

    return output;
}

InitializeVC(tVC *vc, tRobot *r, float p, float i, float d) {
    InitializePID(&(vc->right), p, i, d, -1.0, 1.0);
    InitializePID(&(vc->left), p, i, d, -1.0, 1.0);

    vc->ticksPerLinearVel = r->secsLooprate / r->ticksPerUnit,
    vc->ticksPerAngularVel = r->secsLooprate / (r->ticksPerUnit/r->unitsAxisWidth);
}

