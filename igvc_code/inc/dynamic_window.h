#include "common.h"

#ifndef __DYNAMIC_WINDOW_H__
#define __DYNAMIC_WINDOW_H__

typedef struct {
    float robotRadius;
    float minV, maxV, maxDV, numVSteps;
    float minW, maxW, maxDW, numWSteps;
    float headingWeight, clearanceWeight, speedWeight;
} tDW;

void RunDW(
    tDW *dw,
    tPose *pose,
    tPoint goal,
    tPoint *obstacles,
    int numObstacles,
    tVels *output
    );

#endif // __DYNAMIC_WINDOW_H__
