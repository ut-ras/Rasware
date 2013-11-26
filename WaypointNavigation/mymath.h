#ifndef __MYMATH_H__
#define __MYMATH_H__

// expects angle to be unbounded
// returns angle bounded from 0 to 2PI
// tests: http://codepad.org/HSh2FDXY
float boundAngle2PI(float angle);

// expects a1 and a2 to be unbounded
// returns (a1 - a2) bounded between -PI and PI
// tests: http://codepad.org/HSh2FDXY
float diffAngles(float a1, float a2);

// returns atan2 result bounded from 0 to 2PI
float safe_atan2(float y, float x);

float eulerDistance(float x1, float y1, float x2, float y2);

#endif // __MYMATH_H__
