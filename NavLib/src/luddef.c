// "luddef" stands for localization using differential-drive encoder feedback

#include <math.h>
#include "luddef.h"

#define PI 3.14159265358979323846f

// bounds the angle t to be within the range [0, 2*PI)
//  where t is can be an angle on the range (-inf, inf)
float boundAngle(float t) {
    float nf = t/(2*PI);
    int n = (nf < 0) ? nf - 1 : nf;

    return t - n*2*PI;
}

// deals with small floating-point errors
int floatBasicallyEqual(float a, float b) {
    return fabs(a - b) < 1.0e-6f;
}

void UpdateLUDDEFPose(
    tLUDDEF *luddef,
    tPose *pose,
    signed long leftTicks,
    signed long rightTicks,
    float timeStep
    )
{
    float heading = pose->heading;

    // calculate the distance each wheel has turned
    float leftDist = leftTicks / luddef->ticksPerUnit,
          rightDist = rightTicks / luddef->ticksPerUnit,
          leftDelta = leftDist - luddef->oldLeftDist,
          rightDelta = rightDist - luddef->oldRightDist;
    
    luddef->oldLeftDist = leftDist;
    luddef->oldRightDist = rightDist;

    // this math represents the kinematics of differential steering
    if (floatBasicallyEqual(leftDelta, rightDelta)) {
        pose->x += leftDelta * cos(heading);
        pose->y += rightDelta * sin(heading);
        pose->w = 0.0f;
    } else {
        float R = luddef->unitsAxisWidth * (leftDelta + rightDelta) / (2 * (rightDelta - leftDelta)),
              wd = (rightDelta - leftDelta) / luddef->unitsAxisWidth;
        
        pose->x += R * sin(wd + heading) - R * sin(heading);
        pose->y += -R * cos(wd + heading) + R * cos(heading);
        pose->heading = boundAngle(heading + wd);
        pose->w = wd/timeStep;
    }
    
    pose->v = (leftDelta + rightDelta)/timeStep/2.0f;
}

void InitializeLUDDEF(
    tLUDDEF *luddef,
    float unitsAxisWidth,
    float ticksPerUnit
    )
{
    luddef->unitsAxisWidth = unitsAxisWidth;
    luddef->ticksPerUnit = ticksPerUnit;
    luddef->oldLeftDist = 0;
    luddef->oldRightDist = 0;
}
