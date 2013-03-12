// ***************************
// MOTOR FUNCTIONS AND DEFINES
// ***************************
#ifndef MOTOR_H
#define MOTOR_H

// *************************************************************
// Enum for port selection parameter in motor initializations
// *************************************************************
typdef enum 
{
	GPIOPORTA
	GPIOPORTB
	GPIOPORTC
	GPIOPORTD
	GPIOPORTE
} GPIOPort;	   // GPIO port data types for parameter passing

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

typedef signed char power_t;		// motor power data type

typedef enum {
	MOTOR_0 = PWM_OUT_4,			// MOTOR_0
	MOTOR_1 = PWM_OUT_5				// MOTOR_1
	} motor_t;	// motor data type

// Summary:	Initializes the appropriate PWMs for motor output
// Parameters:
//		invert0:	true if you want to invert MOTOR_0; false otherwise
//		invert1:	true if you want to invert MOTOR_1; false otherwise
// Note:	Always call this function before any other motor-related functions
extern void InitializeMotors(tBoolean invert0, tBoolean invert1);

// Summary: Sets the specified motor's power
// Parameters:
//		motor:		MOTOR_0 or MOTOR_1
//		power:		sets motor to the specified power.
//					0 is neutral, 127 is full forward, -128 is full backward
extern void SetMotorPower(motor_t motor, power_t power);

// Summary: Sets both motor powers simultaneously
// Parameters:
//		power0:		desired power output for MOTOR_0
//		power1:		desired power output for MOTOR_1
extern void SetMotorPowers(power_t power0, power_t power1);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // MOTOR_H
