/*
The point of this file is to generate a smooth path for cyclic servo motion, such as the motion 
that a crawler robot would use.
*/

#ifndef __CUBIC_PATH_MAKER_H__
#define __CUBIC_PATH_MAKER_H__

#define SECTOR_BUFF_SIZE 10

// this could be put in internal.h
struct tCFSector {
    float a0, a1, a2, a3;
    float startTime, timeLen;
    struct tCFSector *next;
};

typedef struct tCFSector tCFSector;

struct {
    float totalTime;
    tCFSector *first;
} typedef tCubicFunct;

struct {
    float pos;
    float time;
    float slope;
} typedef tServoAct;

float GetCFValue(tCubicFunct *cf, float t);

void InitCubicFunct(tCubicFunct *out_cf, tServoAct *actArr, int numActs);

#endif

