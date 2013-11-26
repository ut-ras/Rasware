#include <RASLib/inc/time.h>
#include "localization.h"

void updateLuddefIteration(LuddefData* data) {
    float start = GetTime();
    
    UpdateLUDDEF(
        &(data->luddef),
        GetEncoder(data->leftEnc),
        GetEncoder(data->rightEnc),
        .1
        );
    
    data->time = GetTime() - start;
    (data->count)++;
}
