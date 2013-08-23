//*****************************************************************************
//
// uart.c - usbuart commuications
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

#include <stdarg.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/lm4f120h5qr.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/uart.c"
#include "utils/uartstdio.c"
#include "uart.h"

static const char * const g_pcHex_U = "0123456789ABCDEF";
static const char * const g_pcHex_L = "0123456789abcdef";



// Sets up a simple console through UART0
void InitializeUART(void)
{
    // Enable GPIO port A which is used for UART0 pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the pin muxing for UART0 functions on port A0 and A1.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    // Select the alternate (UART) function for these pins.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.
    StdioInit(0);
}


void StdioConfig(unsigned long ulPort, unsigned long ulBaud, unsigned long ulSrcClock)
{
  UARTStdioConfig(ulPort, ulBaud, ulSrcClock);
}

void StdioInit(unsigned long ulPort)
{
  UARTStdioInit(ulPort);
}

void StdioInitExpClk(unsigned long ulPort, unsigned long ulBaud)
{
  UARTStdioInitExpClk(ulPort, ulBaud);
}

int gets(char *pcBuf, unsigned long ulLen)
{
  return UARTgets(pcBuf, ulLen);
}

unsigned char getc(void)
{
  return UARTgetc();
}

int keyWasPressed(void) {
  if (UARTCharsAvail(UART0_BASE))  
    return 1;
  else
    return 0;
}

void convert(unsigned long ulValue, unsigned long ulCount, const char *pcHex, char cNeg, char cFill, unsigned long ulBase)
{
  char pcBuf[16];
  unsigned long ulIdx, ulPos = 0;
  
  for(ulIdx = 1;
      (((ulIdx * ulBase) <= ulValue) &&
      (((ulIdx * ulBase) / ulBase) == ulIdx));
      ulIdx *= ulBase, ulCount--)
  {
  }
  
  // If the value is negative, reduce the count of padding
  // characters needed.
  if(cNeg)
  {
      ulCount--;
  }

  // If the value is negative and the value is padded with
  // zeros, then place the minus sign before the padding.
  if(cNeg && (cFill == '0'))
  {
      // Place the minus sign in the output buffer.
      pcBuf[ulPos++] = '-';

      // The minus sign has been placed, so turn off the
      // negative flag.
      cNeg = 0;
  }

  // Provide additional padding at the beginning of the
  // string conversion if needed.
  if((ulCount > 1) && (ulCount < 16))
  {
      for(ulCount--; ulCount; ulCount--)
      {
          pcBuf[ulPos++] = cFill;
      }
  }

  // If the value is negative, then place the minus sign
  // before the number.
  if(cNeg)
  {
      // Place the minus sign in the output buffer.
      pcBuf[ulPos++] = '-';
  }

  // Convert the value into a string.
  for(; ulIdx; ulIdx /= ulBase)
  {
      pcBuf[ulPos++] = pcHex[(ulValue / ulIdx) % ulBase];
  }

  // Write the string.
  UARTwrite(pcBuf, ulPos);
}

void printf(const char *pcString, ...)
{
  unsigned long ulValue, ulIdx, ulCount, ulDecCount;
  char *pcStr, cNeg, cDec, cFill;
  const char *pcHex;
  va_list vaArgP;

    // Check the arguments.
    ASSERT(pcString != 0);

    // Start the varargs processing.
    va_start(vaArgP, pcString);

    // Loop while there are more characters in the string.
    while(*pcString)
    {
        // Find the first non-% character, or the end of the string.
        for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
            ulIdx++)
        {
        }

        // Write this portion of the string.
        UARTwrite(pcString, ulIdx);

        // Skip the portion of the string that was written.
        pcString += ulIdx;

        // See if the next character is a %.
        if(*pcString == '%')
        {
            // Skip the %.
            pcString++;

            // Set the digit count to zero, and the fill character to space
            // (i.e. to the defaults).
            ulCount = 0;
            ulDecCount = 6;
            cDec = 0;
            cFill = ' ';
          
            // Presets the template string to lowercase
            pcHex = g_pcHex_L;
          
again:

            // Determine how to handle the next character.
            switch(*pcString++)
            {
                // Handle the digit characters.
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    if((pcString[-1] == '0') && (ulCount == 0))
                    {
                        cFill = '0';
                    }

                    //See if we're after the decimal point
                    if(cDec)
                    {
                      // Update the digit count
                      // Can only print one decimal digit worth of precision
                      ulDecCount = pcString[-1] - '0';
                    }
                    else
                    {
                      // Update the digit count.
                      ulCount *= 10;
                      ulCount += pcString[-1] - '0';
                    }

                    // Get the next character.
                    goto again;
                }
                
                //Handle the . character
                case '.' :
                {
                    cDec = 1;
                    // Get the next character.
                    goto again;
                }

                // Handle the %c command.
                case 'c':
                {
                    // Get the value from the varargs.
                    ulValue = va_arg(vaArgP, unsigned long);

                    // Print out the character.
                    UARTwrite((char *)&ulValue, 1);

                    // This command has been handled.
                    break;
                }

                // Handle the %d and %i commands.
                case 'd':
                case 'i':
                {
                    // Get the value from the varargs.
                    ulValue = va_arg(vaArgP, unsigned long);
                  
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    if((long)ulValue < 0)
                    {
                        // Make the value positive.
                        ulValue = -(long)ulValue;

                        // Indicate that the value is negative.
                        cNeg = 1;
                    }
                    else
                    {
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        cNeg = 0;
                    }

                    // Convert the value to ASCII.
                    convert(ulValue, ulCount, pcHex, cNeg, cFill, 10);
                    break;
                }
                
                // Handle the %o command.
                case 'o':
                {
                    // Get the value from the varargs.
                    ulValue = va_arg(vaArgP, unsigned long);

                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    if((long)ulValue < 0)
                    {
                        // Make the value positive.
                        ulValue = -(long)ulValue;

                        // Indicate that the value is negative.
                        cNeg = 1;
                    }
                    else
                    {
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        cNeg = 0;
                    }

                    // Convert the value to ASCII.
                    convert(ulValue, ulCount, pcHex, cNeg, cFill, 8);
                    break;
                }
                // Handle the %s command.
                case 's':
                {
                    // Get the string pointer from the varargs.
                    pcStr = va_arg(vaArgP, char *);

                    // Determine the length of the string.
                    for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
                    {
                    }

                    // Write the string.
                    UARTwrite(pcStr, ulIdx);

                    // Write any required padding spaces
                    if(ulCount > ulIdx)
                    {
                        ulCount -= ulIdx;
                        while(ulCount--)
                        {
                            UARTwrite(" ", 1);
                        }
                    }
                    // This command has been handled.
                    break;
                }

                // Handle the %u command.
                case 'u':
                {
                    // Get the value from the varargs.
                    ulValue = va_arg(vaArgP, unsigned long);

                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    cNeg = 0;

                    // Convert the value to ASCII.
                    convert(ulValue, ulCount, pcHex, cNeg, cFill, 10);
                    break;
                }

                // Handle the %x and %X commands.  We alias %p to %x.
                case 'X':
                  //Make the template string uppercase
                  pcHex = g_pcHex_U;
                case 'x':
                case 'p':
                {
                    // Get the value from the varargs.
                    ulValue = va_arg(vaArgP, unsigned long);

                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    cNeg = 0;
                  
                    // Convert the value to ASCII.
                    convert(ulValue, ulCount, pcHex, cNeg, cFill, 16);
                    break;
                }

                // Handle the %f and %F commands.
                case 'F': // Not different
                case 'f':
                {
                    //Declare and read a double
                    double dValue;
                    dValue = va_arg(vaArgP, double);
                  
                    //Check if the value is negative
                    if(dValue < 0)
                    {
                        cNeg = 1;
                        dValue = 0 - dValue;
                    }
                    else
                    {
                        cNeg = 0;
                    }
                  
                    // Convert the integer value to ASCII.
                    convert((long)dValue, ulCount, pcHex, cNeg, cFill, 10);
                    //Remove the original integer value and multiply to move decimal places forward
                    dValue = (dValue - (float)((long)dValue));
                    //This loop clobbers ulCount, but it gets reset before we need it again
                    for(ulCount = 0; ulCount < ulDecCount; ulCount++)
                    {
                      dValue *= 10;
                    }
                    UARTwrite(".", 1);
                    convert((long)dValue, ulDecCount, pcHex, 0, cFill, 10);
                    break;
                }
                
                // Placeholder for remaining floating point flags
                case 'E':
                case 'A':
                  //Make the template string uppercase
                  pcHex = g_pcHex_U;
                case 'a':
                case 'e':
                {
                    //This union is needed to get at the bits of the double
                    union dl {
                        double d;
                        long l[2];
                    } dlValue;
                    
                    //Declare and read a double, and load it into the union for now
                    double dValue;
                    dValue = va_arg(vaArgP, double);
                    dlValue.d = dValue;

                    // This formatting shows raw hex value
                    cNeg = 0;
                    cFill = '0';
                    ulCount = 8;
                    convert(dlValue.l[1], ulCount, pcHex, cNeg, cFill, 16);
                    convert(dlValue.l[0], ulCount, pcHex, cNeg, cFill, 16);
                    break;
                }
                
                // Handle the %% command.
                case '%':
                {
                    // Simply write a single %.
                    UARTwrite(pcString - 1, 1);

                    // This command has been handled.
                    break;
                }

                // Handle all other commands.
                default:
                {
                    // Indicate an error.
                    UARTwrite("ERROR", 5);

                    // This command has been handled.
                    break;
                }
            }
        }
      }
}

int write(const char *pcBuf, unsigned long ulLen)
{
  return UARTwrite(pcBuf, ulLen);
}
