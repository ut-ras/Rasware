#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
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
    tMotor *leftMotor;
    tMotor *rightMotor;

    tPID pidController;

    unsigned long prevLeftTicks = 0,
                  prevRightTicks = 0;
    
    tRobot robot = {0};
    robot.unitsAxisWidth = 6.5; // inches
    robot.ticksPerUnit = 1457;  // ticks per inch
    
    InitializeMCU();

    rightMotor = InitializeMotor(PIN_C5, PIN_C4, true, true);
    leftMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);
    SetMotor(leftMotor, 0.0);
    SetMotor(rightMotor, 0.0);

    leftEnc = InitializeEncoder(PIN_B0, PIN_B1, true);
    rightEnc = InitializeEncoder(PIN_E4, PIN_E5, false);

    luddefData.leftEnc = leftEnc;
    luddefData.rightEnc = rightEnc;

    InitializeLUDDEF(&(luddefData.luddef), &robot);
    InitializePID(&pidController, 0.0003, 0.0000000001, 0.0, -1.0, 1.0);

    //CallEvery(updateLuddefIteration, &luddefData, .1);

    while (1) {
        float output;
        long leftTicks = GetEncoder(leftEnc),
             rightTicks = GetEncoder(rightEnc);
        long deltaLeftTicks = leftTicks - prevLeftTicks,
             deltaRightTicks = rightTicks - prevRightTicks;
        prevLeftTicks = leftTicks;
        prevRightTicks = rightTicks;
        
        output = RunPID(&pidController, .5, (float)deltaLeftTicks);
        
        SetMotor(leftMotor, output);
        
        Printf("%3d  %.3f     \n", deltaLeftTicks, output);
        
        Wait(.1);
    }
}
