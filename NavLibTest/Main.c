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


void runControlLoop(tEncoder *enc, tMotor *motor) {
    signed long goalDeltaTicks = 2000;
    
    float prevCommand = 0;
    signed long prevTicks = 0;
    tPID pid = {0};

    InitializePID(&pid, .0001, 0, 0, -1, 1);
    
    while (true) {
        signed long ticks = GetEncoder(enc);
        signed long deltaTicks = ticks - prevTicks;
        
        float motorCommand = prevCommand + RunPID(&pid, goalDeltaTicks, deltaTicks);
        SetMotor(motor, motorCommand);
        
        prevTicks = ticks;
        prevCommand = motorCommand;
        
        Printf("command: %1.2f   deltaTicks: %08d\n", motorCommand, deltaTicks);
        
        Wait(.1);
    }
}

int main(void) {
    tEncoder *rightEnc;
    tMotor *rightMotor;
    
    InitializeMCU();
    
    rightEnc = InitializeEncoder(PIN_D3, PIN_D2, false);
    rightMotor = InitializeMotor(PIN_F3, PIN_F2, true, true);
    
    SetMotor(rightMotor, .2);
    
    /*
    {
    signed long prevTicks = 0;
    
    while (true) {
        signed long ticks = GetEncoder(rightEnc);
        signed long deltaTicks = ticks - prevTicks;
        Printf("deltaTicks: %08d\n", deltaTicks);
        prevTicks = ticks;
        
        Wait(.1);
    }
    }
    */
    
    runControlLoop(rightEnc, rightMotor);
}

int oldmain(void) {
    LuddefData luddefData = {0};
    tEncoder *leftEnc;
    tEncoder *rightEnc;
    tMotor *leftMotor;
    tMotor *rightMotor;

    tPID pidController;

    unsigned long prevLeftTicks = 0,
                  prevRightTicks = 0;
    
    tRobotData robot = {0};
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
