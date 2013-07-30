#include <stdio.h>
#include "cubic_path_maker.h"

static tCFSector sectorBuff[SECTOR_BUFF_SIZE];
static int sectorCount = 0;

static float boundTime(float t, float b) {
    float nf = t/b;
    int n = (nf < 0) ? nf - 1 : nf;
    
    return t - n*b;
}

static float getCFSectorValue(tCFSector *cs, float t) {
    return (cs->a0) + (cs->a1)*t + (cs->a2)*t*t + (cs->a3)*t*t*t;
}

float GetCFValue(tCubicFunct *cf, float t) {
    tCFSector *sector = cf->first;
    t = boundTime(t, cf->totalTime);
    
    while (sector) {
        float start = sector->startTime,
              length = sector->timeLen;
              
        if (t >= start && t <= start + length) {
            return getCFSectorValue(sector, t - start);
        }
        
        sector = sector->next;
    }
    
    return -1;
}

static void SetCubicCoefficients(
    tCFSector *servo,
    float p0,
    float p1,
    float d0,
    float d1,
    float tf
    )
{
    //
    // The ai constants here come from these equations:
    // 
    //  position(t) = a0 + a1*t + a2*t^2 + a3*t^3
    //  velocity(t) = position'(t) = a1 + 2*a2*t + 3*a3*t^2
    //
    // Given the following (provided in the parameters):
    //
    //  p0 = position(t=0)
    //  p1 = position(t=tf)
    //  d0 = velocity(t=0)
    //  d1 = velocity(t=tf)
    //  tf = time the move should take
    //
    servo->a0 = p0;
    servo->a1 = d0;
    servo->a3 = (d1 + d0 - 2*(p1 - p0)/tf)/(tf*tf);
    servo->a2 = (p1 - p0 - d0*tf - (servo->a3)*tf*tf*tf)/(tf*tf);
}

void InitCubicFunct(tCubicFunct *out_cf, tServoAct *actArr, int numActs) {
    int i;
    float totalTime = 0;
    tCFSector *first,
              *sector;

    if (sectorCount + numActs > SECTOR_BUFF_SIZE) {
        printf("error: sector buffer full\n");
        return;
    }

    // allocate and link sectors
    for (i = 0; i < numActs; i++) {
        if (i == 0) {
            first = &sectorBuff[sectorCount++];
            sector = first; 
        } else {
            sector->next = &sectorBuff[sectorCount++];
            sector = sector->next;
        }
    }    

    // make the last sector point to the first one
    sector->next = first;

    // run through actArr and compute slopes for each action
    for (i = 0; i < numActs; i++) {
        int nexti = (i + 1) % numActs;
        actArr[nexti].slope = (actArr[nexti].pos - actArr[i].pos) / actArr[i].time;
    } 

    // calculate cubic equation coefficients for each set of actions, and add up the total time    
    totalTime = 0;
    sector = first;

    for (i = 0; i < numActs; i++) {
        int nexti = (i + 1) % numActs;
        
        SetCubicCoefficients(
            sector,
            actArr[i].pos,
            actArr[nexti].pos,
            actArr[i].slope,
            actArr[nexti].slope,
            actArr[i].time
            );

        sector = sector->next;
        totalTime += actArr[i].time;
    }    

    out_cf->totalTime = totalTime;
    out_cf->first = first;
}

