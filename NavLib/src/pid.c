#include "pid.h"

float RunPID(tPID *pid, float desired, float current) {
    float error = desired - current;
    float diff = error - pid->prevError;

    pid->accumErr += error;
    pid->prevError = error;

    float output = (pid->p) * error + (pid->d) * diff + (pid->i) * pid->accumError;

    if (output < pid->minOutput) {
        output = pid->minOutput;
    } else if (output > pid->maxOutput) {
        output = pid->maxOutput;
    }

    return output;
}

InitializePID(tPID* pid, float p, float i, float d, float min, float max) {
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->minOutput = min;
    pid->maxOutput = max;
    pid->prevErr = 0;
    pid->accumErr = 0;
}
