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

#include "common.h"

typedef struct {
    tRobotData *r;
    float oldLeftDist;
    float oldRightDist;
} tLUDDEF;

// ------------
// UpdateLUDDEF
// ------------

/**
 * Updates the luddef pose and velocity estimate given left and right encoder ticks
 * @param luddef pointer to a luddef struct where the output of this function is placed
 * @param leftTicks current encoder tick count returned by reading the left encoder
 * @param rightTicks current encoder tick count returned by reading the right encoder
 * @param timeStep how much time (in seconds) that has passed since the last call to this function
 * Note: timeStep must not be zero
 */
void UpdateLUDDEF(
    tLUDDEF *luddef,
    signed long leftTicks,
    signed long rightTicks,
    float timeStep
    );

// ----------------
// InitializeLUDDEF
// ----------------

/**
 * Initializes a new luddef struct
 * @param luddef pointer to a luddef struct (allocated by caller)
 * @param luddef pointer to initialized robot struct
 */
void InitializeLUDDEF(
    tLUDDEF *luddef,
    tRobotData *r
    );

#endif // __LUDDEF_H__

