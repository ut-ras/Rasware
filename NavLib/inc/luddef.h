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

#ifndef __LUDDEF_H__
#define __LUDDEF_H__

typedef struct {
    float x;        // position in units
    float y;        // position in units
    float heading;  // direction in radians
    float v;        // linear velocity in units/second
    float w;        // angular velocity in radians/second
} tPose;

typedef struct {
    float unitsAxisWidth;
    float ticksPerUnit;
    float oldLeftDist;
    float oldRightDist;
} tLUDDEF;

void UpdateLUDDEFPose(
    tLUDDEF *luddef,
    tPose *pose,
    signed long leftTicks,
    signed long rightTicks,
    float timeStep // seconds
    );

void InitializeLUDDEF(
    tLUDDEF *luddef,
    float unitsAxisWidth, // where 'units' could be inches, meters, etc.
    float ticksPerUnit    // units must be consistent with axis width
    );

#endif // __LUDDEF_H__
