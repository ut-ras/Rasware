//*****************************************************************************
//
// servo.c - software servo driver
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE AUTHORS OF THIS FILE
// SHALL NOT, UNDER ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of RASLib Rev0 of the RASWare2013 package.
//
// Written by: 
// The student branch of the 
// IEEE - Robotics and Automation Society 
// at the University of Texas at Austin
//
// Website: ras.ece.utexas.edu
// Contact: rasware@ras.ece.utexas.edu
//
//*****************************************************************************

#include "servo.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

void InitializeServoGenerator(void)
{
    int i;
    // Initialze the servo generator buffer to 0 (0 = inactive)
    for( i = 0; i < SERVO_FUNCTION_BUFFER_SIZE; i++){
        rgServoFunctions[i].value = 0;
    }
    
    // Enable SysCtrl for Timer5
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4); 
    
    // Configure Timer4A to be periodic, maintaining the configuration for Timer4B
    TimerConfigure(TIMER4_BASE, TIMER4_CFG_R | TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	
    // Enable the Timer4A interrupt
    IntEnable(INT_TIMER4A);
    TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT );
    
    // Load Timer4A with a frequency of SERVO_GENERATOR_RESOLUTION * SERVO_GENERATOR_RATE
    TimerLoadSet(TIMER4_BASE, TIMER_A, SysCtlClockGet() / SERVO_GENERATOR_RESOLUTION / SERVO_GENERATOR_RATE);
    
    // Enable Timer4A
    TimerEnable(TIMER4_BASE, TIMER_A);
}

// Sets the Servo generator's output to the specified value
// Servo output is 50hz with 2.5% to 12.5% duty cycle, centered at 7.5%
// \param index is the servo to set
// \param input is the value to set at, 0 being 2.5% duty cycle and 255 being 12.5%
tServoFunction rgServoFunctions[SERVO_FUNCTION_BUFFER_SIZE];
void SetServoPosition(unsigned long index, unsigned char input){
    rgServoFunctions[index].value = ((((unsigned long) input) * (SERVO_GENERATOR_RESOLUTION / 10)) >> 8 )+ (SERVO_GENERATOR_RESOLUTION / 40);
}

// Declares a new servo function pin
// Servo pins are run by bit-banging in software
// \param port is the port base (ex. GPIO_PORTA_BASE)
// \param pin is the pin in that port (ex. GPIO_PIN_0)
// \return the index of the servo in the buffer
// 
// Note: Uses Timer4A. Do not use elsewhere is this function is called
unsigned long AddServoFunction( unsigned long port, unsigned long pin ){
    static unsigned long count = 0;
    static tBoolean fIsServoFuncInitialized = false;
    
    // Check to see if the servo generator is initialized. If not, initialize.
    if(!fIsServoFuncInitialized){
        InitializeServoGenerator();
        
        //  Flag as initialized
        fIsServoFuncInitialized = true;
    }
    
    // Find the next availible spot in the buffer
    GPIOPinTypeGPIOOutput(port, pin); 
    
    // Put the new task in the buffer
    rgServoFunctions[count].port = port;
    rgServoFunctions[count].pin = pin;
    SetServoPosition(count, 128);
    GPIOPinWrite(port, pin, 0);
    return count++;
}

void ServoGeneratorHandler(void)
{
    static unsigned long cServoGenTime = 0;
    int i = 0;
    // Increment a counter in terms of servo generator time
    cServoGenTime = ((cServoGenTime+1)%SERVO_GENERATOR_RESOLUTION);
    
    // Iterate through the servo functions
    while( i != SERVO_FUNCTION_BUFFER_SIZE && rgServoFunctions[i].value != 0)
    {
        if(cServoGenTime == 0){ 
            // If the servo time is 0, assert all the servo pins high
            GPIOPinWrite(rgServoFunctions[i].port,rgServoFunctions[i].pin, rgServoFunctions[i].pin );
        }
        else if(cServoGenTime == rgServoFunctions[i].value){
            // If the servo time is a pin's setpoint, assert that pin low
            GPIOPinWrite(rgServoFunctions[i].port,rgServoFunctions[i].pin, 0 );
        }
        i++;	
    }
    
    // Clear the interrupt
    TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
}

