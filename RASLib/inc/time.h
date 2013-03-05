//*****************************************************************************
//
// time.h - real time based functions
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

#define US_PER_SEC 1000000
#define US_PER_MS 1000
#define PERIODIC_FUNCTION_RATE 1000


unsigned long GetTime(void);
unsigned long GetTimeUS(void);
void InitializeSystemTime(void);

int AddPeriodicFunction(void(*task_in)(void), unsigned long period_in);

void WaitUS(unsigned long long ms);
#define Wait WaitUS // Wait is synonymous to WaitUS
void WaitMS(unsigned long long ms);
void WaitS(unsigned long ms);
