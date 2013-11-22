#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/time.h>
#include <NavLib/inc/luddef.h>
#include <NavLib/inc/vel_control.h>

struct LuddefData {
    tLUDDEF luddef;
    
    tEncoder* leftEnc;
    tEncoder* rightEnc;
    
    int count;
    float time;
} typedef LuddefData;

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

int main(void) {
    LuddefData luddefData = {0};
    tEncoder *leftEnc;
    tEncoder *rightEnc;

    tVC velController;
    
    tRobot robot = {0};
    robot.unitsAxisWidth = 6.5; // inches
    robot.ticksPerUnit = 1457;  // ticks per inch
    
    InitializeMCU();

    leftMotor = InitializeMotor(PIN_B7, PIN_B6, false, false);
    leftMotor = InitializeMotor(PIN_C4, PIN_C3, false, false);

    leftEnc = InitializeEncoder(PIN_B0, PIN_B1, true);
    rightEnc = InitializeEncoder(PIN_E4, PIN_E5, false);

    luddefData.leftEnc = leftEnc;
    luddefData.rightEnc = rightEnc;

    InitializeLUDDEF(&(luddefData.luddef), &robot);
    InitializeVC(&velController, &robot, 1.0, 0.0, 0.0, -1.0, 1.0);

    CallEvery(updateLuddefIteration, &luddefData, .1);

    while (1) {
        tVels desired = {1.0, 0};
        tVCAction action = RunVC(
            &velController, 
            &desired, 
            GetEncoder(leftEnc),
            GetEncoder(rightEnc),
            .1
            );
        
        /*
        Printf("%d: x: %3.3f  y: %3.3f  heading: %3.3f calc time: %.5f\n",
            luddefData.count,
            robot.pose.x,
            robot.pose.y,
            robot.pose.heading*180/PI,
            luddefData.time
            );
        */
        
        Wait(.1);
    }
}
