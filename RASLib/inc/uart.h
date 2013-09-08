//*****************************************************************************
//
// uart - USB UART Communications
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
// Contact: ut.ieee.ras@gmail.com
//
//*****************************************************************************

#ifndef _R_UART_H_
#define _R_UART_H_

#ifdef __cplusplus
extern "C" {
#endif


// Must be called before using other UART commands
void InitializeUART(void);

// These are functions for getting UART input
int KeyWasPressed(void);
int Gets(char *pcBuf, unsigned long ulLen);
unsigned char Getc(void);

// These are functions for generating UART output
void Printf(const char *pcString, ...);
void Putc(char ch);
void Puts(const char *pcString);

// These functions configure UART but aren't a part of standard use
void StdioConfig(unsigned long ulPort, unsigned long ulBaud, unsigned long ulSrcClock);
void StdioInit(unsigned long ulPort);
void StdioInitExpClk(unsigned long ulPort, unsigned long ulBaud);


#ifdef __cplusplus
}
#endif

#endif // _R_UART_H_
