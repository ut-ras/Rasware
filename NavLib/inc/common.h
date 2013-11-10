#ifndef __COMMON_H__
#define __COMMON_H__

typedef struct {
    float x;        // position in units
    float y;        // position in units
    float heading;  // direction in radians
    float v;        // linear velocity in units/second
    float w;        // angular velocity in radians/second
} tPose;

typedef struct {
    float v, w;
} tVels;

typedef struct {
    float x, y;
} tPoint;

typedef struct {
    float ticksPerUnit;
    float unitsAxisWidth;
    float secsLoopRate;
} tRobot;

#endif // __COMMON_H__
