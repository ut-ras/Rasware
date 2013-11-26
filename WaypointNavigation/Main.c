#include <math.h>

#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>
#include <NavLib/inc/luddef.h>

#include "localization.h"
#include "control.h"

#define UNITS_AXIS_WIDTH 6.5f
#define TICKS_PER_UNIT 1457.0f
#define CLOSE_ENOUGH_TO_WAYPOINT 0.1f

float eulerDistance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1,
          dy = y2 - y1;
    
    return sqrt(dx*dx + dy*dy);
}

int main(void) {
    tRobot robotData = {0};
    LuddefData luddefData = {0};

    tEncoder *leftEnc;
    tEncoder *rightEnc;
    tMotor *leftMotor;
    tMotor *rightMotor;

    ControlData controlData = {0};

    tPoint waypoints[2] = {{0.0f, 0.0f}, {10.0f, 0.0f}};
    int numWaypoints = sizeof(waypoints)/sizeof(tPoint);
    int waypointIndex = 0;
    
    //
    // Initialize uController components
    //
    InitializeMCU();

    rightMotor = InitializeMotor(PIN_C5, PIN_C4, true, true);
    leftMotor = InitializeMotor(PIN_F3, PIN_F2, true, false);

    SetMotor(leftMotor, 0.0);
    SetMotor(rightMotor, 0.0);

    leftEnc = InitializeEncoder(PIN_B0, PIN_B1, true);
    rightEnc = InitializeEncoder(PIN_E4, PIN_E5, false);
    
    //
    // Localization settup
    //
    robotData.unitsAxisWidth = UNITS_AXIS_WIDTH; // inches
    robotData.ticksPerUnit = TICKS_PER_UNIT;     // ticks per inch

    luddefData.leftEnc = leftEnc;
    luddefData.rightEnc = rightEnc;

    InitializeLUDDEF(&(luddefData.luddef), &robotData);
    
    CallEvery(updateLuddefIteration, &luddefData, .1);

    //
    // Control loop settup
    //
    controlData.goal = &waypoints[0];
    controlData.pose = &(robotData.pose);
    controlData.leftMotor = leftMotor;
    controlData.rightMotor = rightMotor;
    
    CallEvery(controlIteration, &controlData, .1);
    
    //
    // Waypoint supervision
    //
    while (1) {
        // check to see if we're close enough to the current waypoint
        if (CLOSE_ENOUGH_TO_WAYPOINT < eulerDistance(
            waypoints[waypointIndex].x, waypoints[waypointIndex].y,
            robotData.pose.x, robotData.pose.y
            )) 
        {
            // if we are, set goal to next waypoint
            waypointIndex = (waypointIndex + 1)%numWaypoints;
            controlData.goal = &(waypoints[waypointIndex]);
        }
    }
}
