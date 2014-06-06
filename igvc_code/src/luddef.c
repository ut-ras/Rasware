#include <math.h>
#include "extra_math.h"
#include "luddef.h"

void UpdateLUDDEF(
    tLUDDEF *luddef,
    signed long leftTicks,
    signed long rightTicks,
    float timeStep // seconds
    )
{
    // convenience variables
    tPose *pose = &(luddef->r->pose);
    tVels *vels = &(luddef->r->vels);
    float ticksPerUnit = luddef->r->ticksPerUnit,
          unitsAxisWidth = luddef->r->unitsAxisWidth,
          heading = pose->heading;

    // calculate the distance each wheel has turned
    float leftDist = leftTicks / ticksPerUnit,
          rightDist = rightTicks / ticksPerUnit,
          leftDelta = leftDist - luddef->oldLeftDist,
          rightDelta = rightDist - luddef->oldRightDist;
    
    luddef->oldLeftDist = leftDist;
    luddef->oldRightDist = rightDist;

    // this math represents the kinematics of differential steering
    if (fequals(leftDelta, rightDelta)) {
        pose->x += leftDelta * cos(heading);
        pose->y += rightDelta * sin(heading);
        
        if (vels) {
            vels->w = 0.0f;
        }
    } else {
        float R = unitsAxisWidth * (leftDelta + rightDelta) / (2 * (rightDelta - leftDelta)),
              wd = (rightDelta - leftDelta) / unitsAxisWidth;
        
        pose->x += R * sin(wd + heading) - R * sin(heading);
        pose->y += -R * cos(wd + heading) + R * cos(heading);
        pose->heading = boundAngle2PI(heading + wd);
        
        if (vels) {
            vels->w = wd/timeStep;
        }
    }
    
    if (vels) {
        vels->v = (leftDelta + rightDelta)/timeStep/2.0f;
    }
}

void InitializeLUDDEF(
    tLUDDEF *luddef,
    tRobotData *r
    )
{
    luddef->r = r;
    luddef->oldLeftDist = 0;
    luddef->oldRightDist = 0;
}
