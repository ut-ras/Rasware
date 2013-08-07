//*****************************************************************************
//
// uart.h - usbuart commuications
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


#include "utils/uartstdio.h"

void InitializeUART(void);
void StdioConfig(unsigned long ulPort, unsigned long ulBaud, unsigned long ulSrcClock);
void StdioInit(unsigned long ulPort);
void StdioInitExpClk(unsigned long ulPort, unsigned long ulBaud);
int gets(char *pcBuf, unsigned long ulLen);
unsigned char getc(void);
void printf(const char *pcString, ...);
int write(const char *pcBuf, unsigned long ulLen);
