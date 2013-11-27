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
    
    // bound from -1 to 1
    error = error/PI;
    
    // set motors as a linear function of the error
    if (error <= 0) {
        data->leftMotorValue = MAX_MOTOR_SPEED;
        data->rightMotorValue = MOTOR_EQUALIZER*error*(MAX_MOTOR_SPEED - MIN_MOTOR_SPEED) + MAX_MOTOR_SPEED;
    } else if (error > 0) {
        data->leftMotorValue = error*(MIN_MOTOR_SPEED - MAX_MOTOR_SPEED) + MAX_MOTOR_SPEED;
        data->rightMotorValue = MOTOR_EQUALIZER*MAX_MOTOR_SPEED;
    }
    
    SetMotor(data->leftMotor, data->leftMotorValue);
    SetMotor(data->rightMotor, data->rightMotorValue);

    data->time = GetTime() - start;
    (data->count)++;
}
