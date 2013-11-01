#ifndef __PID_H__
#define __PID_H__

typedef struct {
    float p, i d;
    float prevErr, accumErr;
} tPID;

float RunPID(tPID *pid, float desired, float current);

InitializePID(tPID* pid, float p, float i, float d);

#endif // __PID_H__
