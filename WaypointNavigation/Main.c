#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>

#include <NavLib/inc/luddef.h>
#include <NavLib/inc/extra_math.h>

#include "localization.h"
#include "control.h"
#include "test.h"

#define UNITS_AXIS_WIDTH         6.5f     // inches
#define TICKS_PER_UNIT           1457.0f  // ticks per inch
#define CLOSE_ENOUGH_TO_WAYPOINT 3.0f     // inches
#define LOOP_PERIOD              0.2f     // seconds

/*
int main(void) {
    tEncoder* leftEnc = 0;
    tEncoder* rightEnc = 0;
    tMotor* leftMotor = 0;
    tMotor* rightMotor = 0;

    tRobotData robotData = {0};
    LuddefData luddefData = {0};

    //
    // Initialize uController components
    //
    InitializeMCU();

    leftMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);
    rightMotor = InitializeMotor(PIN_C5, PIN_C4, true, true);

    SetMotor(leftMotor, 0.0);
    SetMotor(rightMotor, 0.0);

    leftEnc = InitializeEncoder(PIN_B0, PIN_B1, true);
    rightEnc = InitializeEncoder(PIN_E4, PIN_E5, false);

    //
    // Localization settup
    //
    robotData.unitsAxisWidth = UNITS_AXIS_WIDTH; 
    robotData.ticksPerUnit = TICKS_PER_UNIT;     

    luddefData.leftEnc = leftEnc;
    luddefData.rightEnc = rightEnc;

    InitializeLUDDEF(&(luddefData.luddef), &robotData);

    CallEvery(updateLuddefIteration, &luddefData, LOOP_PERIOD);

    //
    // Control loop
    //
    {
    float left = .2, 
          right = .2;
    long prevLeftTicks = 0, prevRightTicks = 0;
        
    
    while (1) {
        long leftTicks = GetEncoder(leftEnc),
             rightTicks = GetEncoder(rightEnc);
        
        long diffLeft = leftTicks - prevLeftTicks,
             diffRight = rightTicks - prevRightTicks;
        
        float div = (float)diffLeft/(float)diffRight;
        
        if (diffRight == 0) {
            div = 1;
        }
        
        prevLeftTicks = leftTicks;
        prevRightTicks = rightTicks;
        
        Printf("diffLeft: %6d  diffRight: %6d  div: %1.3f\n", 
            diffLeft,  
            diffRight,
            div
            );
        
        SetMotor(leftMotor, left);
        SetMotor(rightMotor, right*.6);

        Wait(LOOP_PERIOD);
    }
    }
}
*/

int main(void) {
    tEncoder* leftEnc = 0;
    tEncoder* rightEnc = 0;
    tMotor* leftMotor = 0;
    tMotor* rightMotor = 0;

    tRobotData robotData = {0};
    LuddefData luddefData = {0};
    ControlData controlData = {0};

    // waypoints defined here in inches
    tPoint waypoints[] = {
        {0.0f, 0.0f}, 
        {15.0f, 0.0f}, 
        {15.0f, 15.0f}, 
        {0.0f, 15.0f}
    };
    int numWaypoints = sizeof(waypoints)/sizeof(tPoint);
    int waypointIndex = 0;

    //
    // Initialize uController components
    //
    InitializeMCU();

    leftMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);
    rightMotor = InitializeMotor(PIN_C5, PIN_C4, true, true);

    SetMotor(leftMotor, 0.0);
    SetMotor(rightMotor, 0.0);

    leftEnc = InitializeEncoder(PIN_B0, PIN_B1, true);
    rightEnc = InitializeEncoder(PIN_E4, PIN_E5, false);

    //
    // Localization settup
    //
    robotData.unitsAxisWidth = UNITS_AXIS_WIDTH; 
    robotData.ticksPerUnit = TICKS_PER_UNIT;     

    luddefData.leftEnc = leftEnc;
    luddefData.rightEnc = rightEnc;

    InitializeLUDDEF(&(luddefData.luddef), &robotData);

    CallEvery(updateLuddefIteration, &luddefData, LOOP_PERIOD);

    //
    // Control loop settup
    //
    controlData.goal = &waypoints[waypointIndex];
    controlData.pose = &(robotData.pose);
    controlData.leftMotor = leftMotor;
    controlData.rightMotor = rightMotor;

    CallEvery(controlIteration, &controlData, LOOP_PERIOD);

    //
    // Waypoint supervisor
    //
    while (1) {
        Printf("localization: %04d %0.5f\n", luddefData.count, luddefData.time);
        Printf("  x: %2.3f  y: %2.3f  heading: %3.1f\n", 
            robotData.pose.x, 
            robotData.pose.y,
            robotData.pose.heading*180/PI);
        Printf("control: %04d %0.5f\n", controlData.count, controlData.time);
        Printf("  left: %1.3f   right: %1.3f\n", 
            controlData.leftMotorValue,
            controlData.rightMotorValue);
        Printf("waypoint %d: %2.1f, %2.1f\n", 
            waypointIndex,
            waypoints[waypointIndex].x, 
            waypoints[waypointIndex].y);
        Printf("\n");
        
        // check to see if we're close enough to the current waypoint
        if (CLOSE_ENOUGH_TO_WAYPOINT > eulerDistance(
            waypoints[waypointIndex].x, waypoints[waypointIndex].y,
            robotData.pose.x, robotData.pose.y
            )) 
        {
            // if we are close enough, set the goal to next waypoint
            waypointIndex = (waypointIndex + 1)%numWaypoints;
            controlData.goal = &(waypoints[waypointIndex]);
        }

        Wait(LOOP_PERIOD);
    }
}
