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

#include "uart.h"

#include <stdarg.h>
#include <math.h>

#include <StellarisWare/inc/hw_types.h>
#include <StellarisWare/inc/hw_ints.h>
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_types.h>
#include <StellarisWare/inc/hw_uart.h>
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/debug.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/driverlib/rom.h>
#include <StellarisWare/driverlib/rom_map.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/uart.h>


static const char * const g_pcHex_L = "0123456789ABCDEF";
static const char * const g_pcHex_U = "0123456789abcdef";

// Sets up a simple console through UART0
void InitializeUART(int baud) {
  // Enable GPIO port A which is used for UART0 pins.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

  // Configure the pin muxing for UART0 functions on port A0 and A1.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);

  // Select the alternate (UART) function for these pins.
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Initialize the UART for console I/O.
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), baud,
			  (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
			   UART_CONFIG_WLEN_8));
}


static unsigned char ungotten = 0;
// Busy-waits for an input character and then returns it
unsigned char Getc(void) {
  unsigned char tmp;
  if( ungotten == 0)
    return UARTCharGet(UART0_BASE);
  else {
    tmp = ungotten;
    ungotten = 0;
    return tmp;
  }
}

void unGetC (unsigned char unGetThis) {
  ungotten = unGetThis;
}

unsigned char whiteSpaceP ( const char toCheck ) {
  switch (toCheck) {
  case ' ':
  case '\t':
  case '\n':
    return true;
  default:
    return false;
  }
}

unsigned char matchCharP (const char toCheck, const char * possible, const int len) {
  int i;
  for (i = 0; i < len; i++) 
    if (toCheck == possible[i])
      return true;
  return false;
}

unsigned int GetAToI () {
  unsigned char tmp;
  unsigned int toRet = 0;
  unsigned char negativeP = false;
  if ((tmp = Getc()) == '-')
    negativeP = true;
  else if (tmp != '+')
    unGetC(tmp);
  while (matchCharP( (tmp = Getc()), "0123456789", 10))
    toRet = (toRet * 10) + (tmp - '0');
  unGetC(tmp);
  if (negativeP) toRet = -toRet;
  return toRet;
}

unsigned int GetXToI () {
  unsigned char tmp;
  unsigned int toRet = 0;
  unsigned char negativeP = false;
  if ((tmp = Getc()) == '-')
    negativeP = true;
  else if (tmp != '+')
    unGetC(tmp);
  while (matchCharP( (tmp = Getc()), "0123456789abcdefABCDEF", 22))
    switch (tmp) {
    case '0': case '1': case '2':
    case '3': case '4': case '5':
    case '6': case '7': case '8':
    case '9': 
      toRet = (toRet * 16) + (tmp - '0');
      break;
    case 'a': case 'b': case 'c':
    case 'd': case 'e': case 'f':
      toRet = (toRet * 16) + 10 + (tmp - 'a');
      break;
    case 'A': case 'B': case 'C':
    case 'D': case 'E': case 'F':
      toRet = (toRet * 16) + 10 + (tmp - 'A');
      break;
    default :
      return 0;
    }
  unGetC(tmp);
  if (negativeP) toRet = -toRet;
  return toRet;
}

unsigned int Scanf(const char * formatString, ... ) {
  int ret = 0;
  int i = -1;
  unsigned char tmp;
  char * s_ptr;
  const char * braket_ptr;
  unsigned int braket_len;
  unsigned int * i_ptr;
  va_list ap;
  va_start(ap, formatString);
  while (formatString[++i] != '\0') {
    switch (formatString[i]) {
    case ' ':
    case '\t':
      while (whiteSpaceP( (tmp = Getc()) ));
      unGetC(tmp);
      break;
    case '%':
    fmtTop:
      switch (formatString[++i]) {
      case '%' :
	if (Getc() == '%') break;
	else goto exit;
      case 'c' :
	*(va_arg(ap, char *)) = Getc();
	break;
      case 's' :
	s_ptr = va_arg(ap, char*);
	while ( ! whiteSpaceP( (*(s_ptr++) = Getc()) ));
	unGetC(s_ptr[-1]);
	s_ptr[-1] = '\0';
	break;
      case '[':
	braket_ptr = &formatString[i+1];
	s_ptr = va_arg(ap, char*);
	while (formatString[++i] != ']' && formatString[i] != '\0');
	braket_len = &formatString[i] - braket_ptr;
	if  (*braket_ptr == '^') {
	  braket_len--;
	  braket_ptr++;
	  while (!matchCharP( (*(s_ptr++) = Getc()), braket_ptr, braket_len));
	}
	else 
	  while (matchCharP( (*(s_ptr++) = Getc()), braket_ptr, braket_len));
	unGetC(s_ptr[-1]);
	s_ptr[-1] = '\0';
	break;
      case 'i':
	i_ptr = va_arg(ap, unsigned int *);
	*i_ptr = GetAToI();
	break;
      case 'x':
	i_ptr = va_arg(ap, unsigned int *);
	*i_ptr = GetXToI();
	break;
      default :
	goto exit;
      }
      ret++;
      break;
    default:
      tmp = Getc();
      if (tmp != formatString[i] ) goto exit;
    }
  }
 exit: 
  va_end(ap);
  return ret;
}

// Busy-waits for input characters over UART and places them into the provided buffer
int Gets(char *buffer, int count) {
    int i;
    
    // Loop while there are more characters.
    for (i = 0; i < count; i++) {
        // Read the next character from the UART.
        buffer[i] = UARTCharGet(UART0_BASE);
        
        // Handle new lines
        if (buffer[i] == '\r' || buffer[i] == '\0') {
            UARTCharPut(UART0_BASE, '\r');
            UARTCharPut(UART0_BASE, '\n');
            buffer[i] = 0;
            return i;
        }
        
        // Echo to the user
        UARTCharPut(UART0_BASE, buffer[i]);
    }
    
    return count;
}

// Outputs a single character over UART
void Putc(char ch) {
    UARTCharPut(UART0_BASE, ch);
}

// Outputs the characters of a null-terminated string over UART
int Puts(const char *buffer, int count) {
    int i;
    
    // Loop while there are more characters to send.
    for (i = 0; i < count; i++) {
        // Handle new lines
        if (buffer[i] == '\n') {
            UARTCharPut(UART0_BASE, '\r');
        } else if (buffer[i] == '\0') {
            return i;
        }
        
        UARTCharPut(UART0_BASE, buffer[i]);
    }
    
    return count;
}

// Checks to see if a character has been received in the UART buffer
int KeyWasPressed(void) {
    return (UARTCharsAvail(UART0_BASE) > 0);
}

// Function to print the value of a unsigned long using the formatting gathered by printf
static void convert(unsigned long ulValue, unsigned long ulCount, const char *pcHex, char cNeg, char cFill, unsigned long ulBase)
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
  Puts(pcBuf, ulPos);
}

// printf taken from StellarisWare with additional flag support
void Printf(const char *pcString, ...)
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
    Puts(pcString, ulIdx);

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

          // See if we're after the decimal point
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
                
        // Handle the . character
        case '.' :
        {
          // Now we're looking at precision value
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
          Puts((char *)&ulValue, 1);

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
          Puts(pcStr, ulIdx);

          // Write any required padding spaces
          if(ulCount > ulIdx)
          {
            ulCount -= ulIdx;
            while(ulCount--)
            {
              Puts(" ", 1);
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
          // Make the template string uppercase
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
          // Declare and read a double
          double dValue;
          dValue = va_arg(vaArgP, double);
        
          // Check if the value is negative
          if(dValue < 0)
          {
            cNeg = 1;
            dValue = 0 - dValue;
          }
          else
          {
              cNeg = 0;
          }
                              
          // Check for out of range constants
          if(isnan(dValue))
          {
            Puts("NaN", 3);
          }
          else if(dValue == INFINITY)
          {
            if(cNeg)
            {
              Puts("-INF", 4);
            }
            else
            {
              Puts("INF", 3);
            }
          }
          else
          {
            // Convert the integer value to ASCII.
            convert((unsigned long)dValue, ulCount, pcHex, cNeg, cFill, 10);
            // Remove the original integer value and multiply to move decimal places forward
            dValue = (dValue - (float)((unsigned long)dValue));
            // This loop clobbers ulCount, but it gets reset before we need it again
            for(ulCount = 0; ulCount < ulDecCount; ulCount++)
            {
              dValue *= 10;
            }
            Puts(".", 1);
            convert((unsigned long)dValue, ulDecCount, pcHex, 0, '0', 10);
          }
          break;
        }
                
        // %E and %e for scientific notation
        case 'E':
          // Make the template string uppercase
          pcHex = g_pcHex_U;
        case 'e':
        {
          // Declare and read a double
          double dValue, dExp, dTmp;
          dValue = va_arg(vaArgP, double);
        
          // Check if the value is negative
          if(dValue < 0)
          {
            Puts("-", 1);
            dValue = 0 - dValue;
          }
                              
          // Check for out of range constants
          if(isnan(dValue))
          {
            Puts("NaN", 3);
          }
          else if(dValue == INFINITY)
          {
            Puts("INF", 3);
          }
          else 
          {
            // Print the most significant digit
            dExp = log10(dValue);
            if(dExp < 0)
            {
              // Handler for negative exponents
              dTmp = dValue / pow(10, (long) dExp - 1);
              cNeg = 1;
              dExp = 0 - dExp;
            }
            else
            {
              dTmp = dValue / pow(10, (long) dExp);
              cNeg = 0;
            }
            Puts(&pcHex[(int)dTmp], 1);
            Puts(".", 1);
            
            // Print ulDecCount following digits
            while(ulDecCount --> 0)
            {
                dTmp -= (long) dTmp;
                dTmp *= 10;
                Puts(&pcHex[(int)dTmp], 1);
            }
            
            // Write the exponent
            Puts(&pcHex[14], 1);
            
            convert((unsigned long)dExp, 0, pcHex, cNeg, cFill, 10);
          }
          break;
        }
                
        // Handle the %% command.
        case '%':
        {
          // Simply write a single %.
          Puts(pcString - 1, 1);

          // This command has been handled.
          break;
        }

        // Handle all other commands.
        default:
        {
          // Indicate an error.
          Puts("ERROR", 5);

          // This command has been handled.
          break;
        }
      }
    }
  }
}


