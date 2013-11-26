#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <RASLib/inc/motor.h>
#include <NavLib/inc/common.h>

#define MAX_MOTOR_SPEED 1
#define MIN_MOTOR_SPEED 0

struct ControlData {
    tPoint* goal; 
    tPose* pose;

    tMotor* leftMotor;
    tMotor* rightMotor;

    int count;
    float time;
} typedef ControlData;

void controlIteration(ControlData* data);

#endif // __CONTROL_H__
