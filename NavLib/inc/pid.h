#ifndef __PID_H__
#define __PID_H__

typedef struct tPID {
    float p;
    float i;
    float d;
    float maxOutput;
    float minOutput;
    float prevError;
    float accumError;
} tPID;

// ------
// RunPID
// ------

/**
 * Runs one iteration of the pid loop
 * @param pid pointer to an intialized pid struct
 * @param desired input value that we wanted to have read
 * @param current input value that was read
 * @return the output of the pid loop, bounded by min and max output specified during initialization
 */
float RunPID(
    tPID *pid, 
    float desired, 
    float current
    );

// -------------
// InitializePID
// -------------

/**
 * Initializes a new pid struct
 * @param pid pointer to a pid struct (allocated by caller)
 * @param p proportional constant of the PID loop
 * @param i integral term of the PID loop
 * @param d differential term of the PID loop
 * @param min lower bound of output
 * @param max upper bound of output
 */
void InitializePID(
    tPID* pid, 
    float p, 
    float i, 
    float d, 
    float min, 
    float max
    );

#endif // __PID_H__

