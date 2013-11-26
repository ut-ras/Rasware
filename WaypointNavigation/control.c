#include <RASLib/inc/time.h>
#include <NavLib/inc/extra_math.h>
#include "control.h"

void controlIteration(ControlData* data) {
    float start = GetTime();

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

    data->time = GetTime() - start;
    (data->count)++;
}
