#include <math.h>
#include "control.h"

// expects angle to be unbounded
// returns angle bounded from 0 to 2PI
// tests: http://codepad.org/HSh2FDXY
static float boundAngle(float angle) {
    if (angle < 0) {
        int counts = 1 + (int)(-angle/(2*PI));
        angle += counts*2*PI;
    } else if (angle > 2*PI) {
        int counts = (int)(angle/(2*PI));
        angle -= counts*2*PI;
    }
    
    return angle;
}

// expects a1 and a2 to be unbounded
// returns (a1 - a2) bounded between -PI and PI
// tests: http://codepad.org/HSh2FDXY
static float diffAngles(float a1, float a2) {
    float diff;
    
    a1 = boundAngle(a1);
    a2 = boundAngle(a2);

    diff = a1 - a2;

    if (diff > PI) {
        diff = -(2*PI - diff);
    } else if (diff < -PI) {
        diff = 2*PI + diff;
    }

    return diff;
}

// returns atan2 result bounded from 0 to 2PI
static  float safe_atan2(float y, float x) {
    // protect against "domain error"
    if (y == x && y == 0) {
        return 0;
    }
    
    return atan2(y, x);
}

void controlIteration(ControlData* data) {
    // compute desired heading towards goal
    float goalHeading = safe_atan2(
        data->goal->y - data->pose->y, 
        data->goal->x - data->pose->x
        );
    
    // compute error between our current heading and the goal heading
    float error = diffAngles(goalHeading, data->pose->heading);
    
    // bound from -.5 to .5
    error = error/(2*PI);
    
    // set motors as a linear function of the error
    // TODO: fix these to be a function of min and max motor speeds
    SetMotor(data->leftMotor, .5f - error); 
    SetMotor(data->rightMotor, .5f + error);
}
