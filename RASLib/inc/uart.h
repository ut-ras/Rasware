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

        typedef struct tUART tUART;

#define UART_COUNT 8

/**
 * Must be called before using other UART commands
 * @param baud Baud rate of the UART channel
 */
        void InitializeUART(int baud);
        tUART * InitializeUARTModule(int module,int baud);
    
/**
 * Determines correct alignment for doubles passed into printf.
 */    
void InitializeDoublePrintHack(void);

/**
 * Checks to see if a character has been received in the UART buffer
 * @return 1 if a character was found, 0 otherwise
 */
int KeyWasPressed(void);
    
/**
 * Busy-waits for an input character and then returns it
 * @return Character received
 */
unsigned char Getc(void);

/**
 * Busy-waits for input characters over UART and places them into the provided buffer
 * TODO: finish
 * @param pcBuf
 * @param ulLen
 * @return
 */
int Gets(char *buffer, int count);

/**
 * TODO: write
 */
        unsigned int Scanf(const char * formatString, ... );
        unsigned int fScanf(tUART * module, const char * formatString, ... );

/**
 * TODO: write
 */
        void SetEcho(tUART * module, unsigned char on);

/**
 * Outputs a single character over UART
 * @param ch Character to output
 */
        void Putc( char ch);

/**
 * Outputs the characters of a null-terminated string over UART
 * @param pcString String of character to output
 */
int Puts(const char *buffer, int count);

/**
 * Formats a string and outputs it over UART
 * @param pcString Format string
 * @param ... Additional parameters to be inserted into the format string
 */
        void Printf(const char *pcString, ...);
        void fPrintf(tUART * module, const char *pcString, ...);

// These functions configure UART but aren't a part of standard use
/**
 * TODO: finish
 * @param ulPort
 * @param ulBaud
 * @param ulSrcClock
 */
void StdioConfig(unsigned long ulPort, unsigned long ulBaud, unsigned long ulSrcClock);

/**
 * TODO: finish
 * @param ulPort
 */
void StdioInit(unsigned long ulPort);

/**
 * TODO: finish
 * @param ulPort
 * @param ulBaud
 */
void StdioInitExpClk(unsigned long ulPort, unsigned long ulBaud);


#ifdef __cplusplus
}
#endif

#endif // _R_UART_H_
