#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__

#include <RASLib/inc/encoder.h>
#include <NavLib/inc/luddef.h>

struct LuddefData {
    tLUDDEF luddef;
    
    tEncoder* leftEnc;
    tEncoder* rightEnc;
    
    int count;
    float time;
} typedef LuddefData;

void updateLuddefIteration(LuddefData* data);

#endif // __LOCALIZATION_H__
