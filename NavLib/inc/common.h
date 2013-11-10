#ifndef __COMMON_H__
#define __COMMON_H__

#define PI 3.14159265358979323846f

typedef struct {
    float x;        // position in units
    float y;        // position in units
    float heading;  // direction in radians
} tPose;

typedef struct {
    float v;        // linear velocity in units/second
    float w;        // angular velocity in radians/second
} tVels;

typedef struct {
    float unitsAxisWidth; // where 'units' could be inches, meters, etc.
    float ticksPerUnit;   // units must be consistent with axis width
    tPose pose;
    tVels vels;
} tRobot;

typedef struct {
    float x, y;
} tPoint;

#endif // __COMMON_H__

