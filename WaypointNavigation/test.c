#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
#include <NavLib/inc/luddef.h>

#include "control.h"
#include "test.h"

static void printWrongValues(ControlData* controlData) {
    Printf("wrong: left: %1.3f   right: %1.3f\n", 
        controlData->leftMotorValue,
        controlData->rightMotorValue);
}

static void printValues(ControlData* controlData) {
    Printf("left: %1.3f   right: %1.3f\n", 
        controlData->leftMotorValue,
        controlData->rightMotorValue);
}

void testControl(void) {
    tBoolean passed = true;
    
    tPose pose = {0};
    tPoint goal = {0};
    ControlData controlData = {0};

    InitializeMCU();

    goal.x = 10;
    goal.y = 0;

    pose.x = 0;
    pose.y = 0;
    pose.heading = 0;
    
    controlData.goal = &goal;
    controlData.pose = &pose;
    controlData.leftMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);
    controlData.rightMotor = InitializeMotor(PIN_C5, PIN_C4, true, true);

    controlIteration(&controlData);
    if (!(controlData.leftMotorValue == controlData.rightMotorValue)) {
        passed = false;
        printWrongValues(&controlData);
    }
    printValues(&controlData);

    goal.x = 10;
    goal.y = 1;
    
    controlIteration(&controlData);
    if (!(controlData.leftMotorValue < controlData.rightMotorValue)) {
        passed = false;
        printWrongValues(&controlData);
    }
    printValues(&controlData);
    
    goal.x = 10;
    goal.y = -1;
    
    controlIteration(&controlData);
    if (!(controlData.leftMotorValue > controlData.rightMotorValue)) {
        passed = false;
        printWrongValues(&controlData);
    }
    printValues(&controlData);
        
    goal.x = -10;
    goal.y = 1;
    
    controlIteration(&controlData);
    if (!(controlData.leftMotorValue < controlData.rightMotorValue)) {
        passed = false;
        printWrongValues(&controlData);
    }
    printValues(&controlData);
        
    goal.x = -10;
    goal.y = -1;
    
    controlIteration(&controlData);
    if (!(controlData.leftMotorValue > controlData.rightMotorValue)) {
        passed = false;
        printWrongValues(&controlData);
    }
    printValues(&controlData);
    
    if (passed) {
        Printf("everything's good!\n");
    }
    
    while(true);
}
