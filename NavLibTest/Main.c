#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/time.h>
#include <NavLib/inc/luddef.h>

#define PI 3.14159265358979323846

int main(void) {
    tPose pose = {0};
    tLUDDEF luddef;
    tEncoder *rightEnc, *leftEnc;
    int count = 0;
    
    InitializeMCU();

    rightEnc = InitializeEncoder(PIN_B0, PIN_B1, 0);
    leftEnc = InitializeEncoder(PIN_B2, PIN_B3, 1);
    
    // units = inches
    InitializeLUDDEF(&luddef, 7.125, 65.6);
    
    while (1) {
        UpdateLUDDEFPose(
            &luddef,
            &pose,
            GetEncoder(leftEnc),
            GetEncoder(rightEnc),
            .1
            );
        
        Printf("%d: x*100: %04d  y*100: %04d  heading: %04d\r",
            count++,
            (int)(pose.x*100.0f),
            (int)(pose.y*100.0f),
            (int)(pose.heading*180.0f/(float)PI)
            );
        
        Wait(.1);
    }
}
