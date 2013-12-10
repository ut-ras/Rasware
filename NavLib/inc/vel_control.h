#ifndef __VEL_CONTROL_H__
#define __VEL_CONTROL_H__

#include "common.h"
#include "pid.h"

typedef struct {
    float leftMotor;
    float rightMotor;
} tVCAction;

typedef struct {
    tPID left;
    tPID right;
    tRobotData *r;
    signed long prevLeftTicks;
    signed long prevRightTicks;
    tVCAction prevOutput;
} tVC;

// -----
// RunVC
// -----

/** 
 * Runs one velocity control iteration, returning what values to set left and right motors
 * @param vc pointer to an initialized velocity control struct
 * @param desired pointer to struct describing the target velocities that the caller want to get
 * @param leftTicks current encoder tick count returned by reading the left encoder
 * @param rightTicks current encoder tick count returned by reading the right encoder
 * @param timeStep how much time (in seconds) that has passed since the last call to this function
 * @return VCAction representing what value the left and right motors should be set to
 */
tVCAction RunVC(
    tVC *vc, 
    tVels *desired, 
    signed long leftTicks,
    signed long rightTicks, 
    float timeStep
    );

// ------------
// InitializeVC
// ------------

/**
 * Initializes a new velocity control struct
 * @param vc pointer to velocity control struct (allocated by caller)
 * @param r pointer to robot struct
 * @param leftP proportional constant of left motor's PID loop
 * @param leftI integral term of left motor's PID loop
 * @param leftD differential term of left motor's PID loop
 * @param rightP proportional constant of right motor's PID loop
 * @param rightI integral term of right motor's PID loop
 * @param rightD differential term of right motor's PID loop
 * @param min lower bound of action outputs
 * @param max upper bound of action outputs
 */
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
    );

#endif // __VEL_CONTROL_H__
