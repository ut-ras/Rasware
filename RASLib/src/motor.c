//*****************************************************************************
//
// motor.c - software pwm drivers for the TLE5205-2 
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

#include "motor.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

typedef struct{ 
    unsigned long port0; 
    unsigned long pin0;
    unsigned long port1; 
    unsigned long pin1;
    signed long value0;
    signed long value1;
    tMotorMode mode;
    tBoolean active;
} tMotor;
static tMotor rgMotorFunctions[MOTOR_FUNCTION_BUFFER_SIZE];

void InitializeMotorGenerator(void)
{
    int i;
    // Initialze the motor generator buffer to 0 (0 = inactive)
    for( i = 0; i < MOTOR_FUNCTION_BUFFER_SIZE; i++){
        rgMotorFunctions[i].active = false;
    }
    
    // Enable SysCtrl for Timer4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4); 
    
    // Configure Timer4A to be periodic, maintaining the configuration for Timer4B
    TimerConfigure(TIMER4_BASE, TIMER4_CFG_R | TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
	
    // Enable the Timer4A interrupt
    IntEnable(INT_TIMER4B);
    TimerIntEnable(TIMER4_BASE, TIMER_TIMB_TIMEOUT );
    
    // Load Timer4A with a frequency of MOTOR_GENERATOR_RESOLUTION * MOTOR_GENERATOR_RATE
    TimerLoadSet(TIMER4_BASE, TIMER_B, SysCtlClockGet() / MOTOR_GENERATOR_RESOLUTION / MOTOR_GENERATOR_RATE);
    
    // Enable Timer4A
    TimerEnable(TIMER4_BASE, TIMER_B);
}

// Sets the Motor generator's output to the specified value
// \param index is the motor to set
// \param input is the value to set at, 0 being stopped, 1 being full forward, and -1 being full back;
void SetMotorPosition(unsigned long index, float input){
    if(input > 1 || input < -1) return; // restrict input
    switch(rgMotorFunctions[index].mode)
    {
        case BRAKE: if(input < 0){ // CCW P(~P)
                        rgMotorFunctions[index].value0 = MOTOR_GENERATOR_RESOLUTION * (-input);
                        rgMotorFunctions[index].value1 = MOTOR_GENERATOR_RESOLUTION * input;
                    }
                    else if(input > 0){ // CW P0
                        rgMotorFunctions[index].value0 = MOTOR_GENERATOR_RESOLUTION * input;
                        rgMotorFunctions[index].value1 = 0;
                    }
                    else{ //S 10
                        rgMotorFunctions[index].value0 = MOTOR_GENERATOR_RESOLUTION;
                        rgMotorFunctions[index].value1 = 0;
                    }
                    break;    
        case COAST: if(input < 0){ // CCW P1
                        rgMotorFunctions[index].value0 = MOTOR_GENERATOR_RESOLUTION * (-input);
                        rgMotorFunctions[index].value1 = MOTOR_GENERATOR_RESOLUTION;
                    }
                    else if(input > 0){ // CW PP
                        rgMotorFunctions[index].value0 = MOTOR_GENERATOR_RESOLUTION * input;
                        rgMotorFunctions[index].value1 = MOTOR_GENERATOR_RESOLUTION * input;
                    }
                    else{ //S 11
                        rgMotorFunctions[index].value0 = MOTOR_GENERATOR_RESOLUTION;
                        rgMotorFunctions[index].value1 = MOTOR_GENERATOR_RESOLUTION;
                    }
                    break;
    }
}

// Declares a new motor function pin
// Motor pins are run by bit-banging in software
// \param port is the port base (ex. GPIO_PORTA_BASE)
// \param pin is the pin in that port (ex. GPIO_PIN_0)
// \return the index of the motor in the buffer
// 
// Note: Uses Timer4B. Do not use elsewhere is this function is called
unsigned long AddMotorFunction( unsigned long port0, unsigned long pin0 , unsigned long port1, unsigned long pin1 , tMotorMode mode){
    static unsigned long count = 0;
    static tBoolean fIsMotorFuncInitialized = false;
    
    // Check to see if the motor generator is initialized. If not, initialize.
    if(!fIsMotorFuncInitialized){
        InitializeMotorGenerator();
        
        //  Flag as initialized
        fIsMotorFuncInitialized = true;
    }
    
    // Find the next availible spot in the buffer
    GPIOPinTypeGPIOOutput(port0, pin0); 
    GPIOPinTypeGPIOOutput(port1, pin1);
    
    // Put the new task in the buffer
    rgMotorFunctions[count].port0 = port0;
    rgMotorFunctions[count].pin0 = pin0;
    rgMotorFunctions[count].port1 = port1;
    rgMotorFunctions[count].pin1 = pin1;
    rgMotorFunctions[count].mode = mode;
    rgMotorFunctions[count].active = true;
    SetMotorPosition(count, 0);
    GPIOPinWrite(port0, pin0, pin0);
    GPIOPinWrite(port1, pin1, pin1);
    return count++;
}

void MotorGeneratorHandler(void)
{
    static unsigned long cMotorGenTime = 0;
    int i = 0;
    // Increment a counter in terms of motor generator time
    cMotorGenTime = ((cMotorGenTime+1)%MOTOR_GENERATOR_RESOLUTION);
    
    // Iterate through the motor functions
    while( i != MOTOR_FUNCTION_BUFFER_SIZE && rgMotorFunctions[i].active )
    {
        if(cMotorGenTime == 0){ 
            // If the motor time is 0, assert all the motor pins high/low
            GPIOPinWrite(rgMotorFunctions[i].port0, rgMotorFunctions[i].pin0, rgMotorFunctions[i].value0 > 0 ? rgMotorFunctions[i].pin0 : 0);
            GPIOPinWrite(rgMotorFunctions[i].port1, rgMotorFunctions[i].pin1, rgMotorFunctions[i].value1 > 0 ? rgMotorFunctions[i].pin1 : 0);
        }
        else{ 
            if(cMotorGenTime == rgMotorFunctions[i].value0){
                // If the motor time is a pin's setpoint, assert that pin low/high
                GPIOPinWrite(rgMotorFunctions[i].port0, rgMotorFunctions[i].pin0, rgMotorFunctions[i].value0 > 0 ? 0 : rgMotorFunctions[i].pin0);
            }
            if(cMotorGenTime == rgMotorFunctions[i].value1){
                // If the motor time is a pin's setpoint, assert that pin low/high
                GPIOPinWrite(rgMotorFunctions[i].port1, rgMotorFunctions[i].pin1, rgMotorFunctions[i].value1 > 0 ? 0 : rgMotorFunctions[i].pin1);
            }
        }
        i++;	
    }
    
    // Clear the interrupt
    TimerIntClear(TIMER4_BASE, TIMER_TIMB_TIMEOUT);
}
