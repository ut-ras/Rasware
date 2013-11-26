#include <math.h>
#include "extra_math.h"

// deals with small floating-point errors
int fequals(float a, float b) {
    return fabs(a - b) < 1.0e-6f;
}

// expects angle to be unbounded
// returns angle bounded from 0 to 2PI
// tests: http://codepad.org/HSh2FDXY
float boundAngle2PI(float angle) {
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
float diffAngles(float a1, float a2) {
    float diff;
    
    a1 = boundAngle2PI(a1);
    a2 = boundAngle2PI(a2);

    diff = a1 - a2;

    if (diff > PI) {
        diff = -(2*PI - diff);
    } else if (diff < -PI) {
        diff = 2*PI + diff;
    }

    return diff;
}

// returns atan2 result bounded from 0 to 2PI
float safe_atan2(float y, float x) {
    // protect against "domain error"
    if (y == x && y == 0) {
        return 0;
    }
    
    return atan2(y, x);
}

float eulerDistance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1,
          dy = y2 - y1;
    
    return sqrt(dx*dx + dy*dy);
}
