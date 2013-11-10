/*
"LUDDEF" stands for "localization using differential-drive encoder feedback"

These functions use dead reckoning to estimate the x, y position, heading, angular velocity,
and linear velocity of the center point between two encoders, which are assumed to be attached
to the wheels or motors of a differential-drive (or "tank-drive") robot.

To initialize, provide the distance between the two encoders (unitsAxisWidth) in some "units"
of distance, along with the number of ticks an encoder registers per one unit of distance
(ticksPerUnit). To get a pose estimation, provide a pose, the number of ticks each encoder has
registered, and how much time has passed since the last estimation (timeStep). The estimation
function will place its output in the pose struct provided.

For example usage, see the LuddefTest project.
*/
#include "common.h"

#ifndef __LUDDEF_H__
#define __LUDDEF_H__

typedef struct {
    tRobot *r;
    float oldLeftDist;
    float oldRightDist;
} tLUDDEF;

void UpdateLUDDEFPose(
    tLUDDEF *luddef,
    tRobot *r,
    signed long leftTicks,
    signed long rightTicks,
    float timeStep // seconds
    );

void InitializeLUDDEF(
    tLUDDEF *luddef,
    tRobot *r
    );

#endif // __LUDDEF_H__

