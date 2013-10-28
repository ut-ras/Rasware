#include <RASLib/inc/time.h>

// Pololu Line Sensor code from RASLib2012

// Summary: Initializes the appropriate GPIO
// Note:        Always call this function before any other sensor-related functions
//                        Initializes dischargeTime to a default value of 1000 us
extern void InitializeLineSensor(void);

// Summary: Sets the discharge time
// Parameters:
//                dischargeTime: the new discharge time 
//                constraints: 120 <= dischargeTime <= 2600
extern void SetDischargeTime(unsigned long dischargeTime);

// Summary:        Reads the current line sensor values
// Note:        this method takes 10 us + dischargeTime to execute
// Parameters:
//		ret: a char array which will have either 1s or 0s loaded into the first 8 elements
extern void ReadLineSensor(char *ret);
