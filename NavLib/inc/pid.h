#ifndef __PID_H__
#define __PID_H__

typedef struct tPID {
    float p, i, d;
    float maxOutput, minOutput;
    float prevError, accumError;
} tPID;

float RunPID(
    tPID *pid, 
    float desired, 
    float current
    );

void InitializePID(
    tPID* pid, 
    float p, 
    float i, 
    float d, 
    float min, 
    float max
    );

#endif // __PID_H__

