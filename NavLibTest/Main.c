#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/time.h>
#include <NavLib/inc/luddef.h>

int main(void) {
    tLUDDEF luddef;
    
    tEncoder* leftEnc = InitializeEncoder(PIN_B0, PIN_B1, true);
    tEncoder* rightEnc = InitializeEncoder(PIN_E4, PIN_E5, false);

    int count = 0;

    tRobot robot = {0};
    robot.unitsAxisWidth = 6.5; // inches
    robot.ticksPerUnit = 1457;  // ticks per inch
    
    InitializeMCU();
    InitializeLUDDEF(&luddef, &robot);
    
    while (1) {
        UpdateLUDDEFPose(
            &luddef,
            GetEncoder(leftEnc),
            GetEncoder(rightEnc),
            .1
            );
        
        Printf("%d: x: %3.3f  y: %3.3f  heading: %3.3f\r",
            count++,
            robot.pose.x,
            robot.pose.y,
            robot.pose.heading*180/PI
            );
        
        Wait(.1);
    }
}
