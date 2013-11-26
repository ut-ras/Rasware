#include <math.h>
#include "control.h"

float angleDiff(float a1, float a2) {
    return a1 - a2; // TODO: this is clearly wrong
}

float my_atan2(float y, float x) {
    // protect against "domain error"
    if (y == x && y == 0) {
        return 0;
    }
    
    return atan2(y, x);
}

void controlIteration(ControlData* data) {
    // compute desired heading towards goal
    float goalHeading = my_atan2(
        data->goal->y - data->pose->y, 
        data->goal->x - data->pose->x
        );
    
    // compute error between our current heading and the desired
    float error = angleDiff(goalHeading, data->pose->heading);
    
    // normalize
    error = error/(2*PI);
    
    // set motors as a linear function of the error
    // TODO: fix these to be a function of min and max motor speeds
    SetMotor(data->leftMotor, 1 + error); 
    SetMotor(data->rightMotor, 1 + 1-error);
}
