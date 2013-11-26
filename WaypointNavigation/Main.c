#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>
#include <NavLib/inc/luddef.h>

#include "localization.h"
#include "control.h"
#include "mymath.h"

#define UNITS_AXIS_WIDTH         6.5f     // inches
#define TICKS_PER_UNIT           1457.0f  // ticks per inch
#define CLOSE_ENOUGH_TO_WAYPOINT 0.1f     // inches
#define LOOP_PERIOD              0.1f     // seconds

int main(void) {
    tEncoder *leftEnc;
    tEncoder *rightEnc;
    tMotor *leftMotor;
    tMotor *rightMotor;

    tRobotData robotData = {0};
    LuddefData luddefData = {0};
    ControlData controlData = {0};

    tPoint waypoints[2] = {
        {0.0f, 0.0f}, 
        {10.0f, 0.0f}
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
        // check to see if we're close enough to the current waypoint
        if (CLOSE_ENOUGH_TO_WAYPOINT < eulerDistance(
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
