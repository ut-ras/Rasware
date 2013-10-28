#include "GPIOLineSensor.h"

#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/timer.h>

// Pololu Line Sensor code from RASLib2012

static unsigned long ls_dischargeTime;					// the discharge time in microseconds for the line sensors
static const unsigned ls_chargeTime = 10;				// it takes 10 microseconds to charge the line sensors
static const unsigned ls_defaultDischargeTime = 1000;	// default discharge time in microseconds
static volatile char *last_ls;					// cache last value read by line sensor

/*
	Pin Ordering
	PB5,PD0,PD1,PD2,PD3,PE0,PC6,PC7
*/


void StartChargingLS(void)
{
	// start charging
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xff);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0xff);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0xff);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0xff);
}

void StartDischargingLS(void)
{
	// allow the sensors to discharge
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
}

void ReadLS(volatile char *ret)
{
	ret[0] = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5) >> 5);
	ret[1] = (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0)     );
	ret[2] = (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) >> 1);
	ret[3] = (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) >> 2);
	ret[4] = (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) >> 3);
	ret[5] = (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0)     );
	ret[6] = (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6) >> 6);
	ret[7] = (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7) >> 7);
	
}

void LSIntHandler(void)
{
	static volatile tBoolean readyToRead = true;
	unsigned waitTime;
	unsigned long ulTicksPerUs;

	// Mark interrupt as processed
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ulTicksPerUs = SysCtlClockGet() / US(1);

	if(readyToRead)					// finished waiting to discharge
	{					  
		ReadLS(last_ls);
		waitTime = ulTicksPerUs * ls_chargeTime;
		StartChargingLS();

	} else							// finished charging
	{						 
		waitTime = ulTicksPerUs * ls_dischargeTime;
		StartDischargingLS();
	}

	TimerLoadSet(TIMER0_BASE, TIMER_A, waitTime);
	readyToRead = !readyToRead;		// flip flag
}

// Summary: Initializes the appropriate GPIO
// Note:	Always call this function before any other sensor-related functions
//			Initializes dischargeTime to a default value of 1000 us
void InitializeLineSensor(void)
{
	// Enable the peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

	// default discharge time
	SetDischargeTime(ls_defaultDischargeTime);
}

// Summary: Sets the discharge time
// Parameters:
//		dischargeTime: the new discharge time 
//				constraints: 120 <= dischargeTime <= 2600
void SetDischargeTime(unsigned long dischargeTime)
{
	if(dischargeTime >= 120 && dischargeTime <= 2600)		// bounds check
		ls_dischargeTime = dischargeTime;
}

// Summary:	Reads the current line sensor values
// Note:	this method takes 10 us + dischargeTime to execute
// Parameters:
//		ret: a char array which will have either 1s or 0s loaded into the first 8 elements
void ReadLineSensor(char *ret)
{					   
	StartChargingLS();								   
	// wait until fully charged (10 us)
	WaitUS(ls_chargeTime);

	StartDischargingLS();
	// wait 120 us <= threshold <= 2600 us											 
	WaitUS(ls_dischargeTime);

	ReadLS(ret);
}

