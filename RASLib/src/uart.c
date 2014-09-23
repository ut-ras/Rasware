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


// Character lookup tables for numbers
static const char *upper = "0123456789ABCDEF";
static const char *lower = "0123456789abcdef";

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

// Busy-waits for an input character and then returns it
unsigned char Getc(void) {
    return UARTCharGet(UART0_BASE);
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


// Unfortunately varargs wreaks havoc on how floating point is passed. 
// We get a 64bit double and have to decode it ourselves.
#define va_d2f(args) DoubleFloat(&args.__ap)

static float DoubleFloat(void **args) {
    unsigned int a, b;
    int exp;

    union {
        float f;
        unsigned int i;
    } num;
    
    if (!(((unsigned int)(*args)) & 0x4)) {
        a = (*(unsigned int **)args)[2];
        b = (*(unsigned int **)args)[1];
        (*(unsigned int **)args) += 3;
    } else {
        a = (*(unsigned int **)args)[1];
        b = (*(unsigned int **)args)[0];
        (*(unsigned int **)args) += 2;
    }
    
    exp = (0x7ff & (a >> 20)) - 1023 + 127;
    if (exp > 0xff) exp = 0xff; // check range
    if (exp < 0x00) exp = 0x00;
    
    num.i =  0x80000000 & a; // sign bit
    num.i |= 0x7f800000 & (exp << 23); // exponent
    num.i |= 0x007ffff8 & ((0xfffff & a) << 3); // high mantissa
    num.i |= 0x00000003 & (b >> 29); // low mantissa
    
    return num.f;
}


/** Helper functions for printf **/
static int GetInt(const char **buffer) {
    int res = 0;
    
    while (**buffer >= '0' && **buffer <= '9') {
        res *= 10;
        res += **buffer - '0';
        (*buffer)++;
    }
    
    return res;
}

static void Pad(char c, int n) {
    int i;
    
    for (i = 0; i < n; i++) {
        Putc(c);
    }
}

/** String printing **/
static int SizeString(const char *buffer, int count) {
    int i;
    
    for (i = 0; i < count; i++) {
        if (buffer[i] == '\0')
            return i;
    }
    
    return count;
}

static void PutString(const char *buffer, int left, int width, int prec) {
    int len;
    
    if (prec < 0) prec = 0x7fffffff;
    
    len = SizeString(buffer, prec);
    
    if (len > width) {
        Puts(buffer, prec);
    } else if (left) {
        Puts(buffer, prec);
        Pad(' ', width - len);
    } else {
        Pad(' ', width - len);
        Puts(buffer, prec);
    }
}

/** Integer printing **/
static int SizeNum(int base, unsigned int n) {
    int i = 0;
    
    while (n) {
        i++;
        n /= base;
    }
    
    return i;
}

static void PutNum(const char *table, int base, int count, unsigned int n) {
    if (!n && count <= 0)
        return;
    
    PutNum(table, base, count-1, n/base);
    Putc(table[n%base]);
}

static void PutBase(unsigned int n, const char *table, int base, int left, int sign, int width) {
    if (sign == 1) {
        Putc('+');
        width--;
    } else if (sign == 2) {
        Putc(' ');
        width--;
    }
    
    if (width < 1) 
        width = 1;
    
    if (left) {
        PutNum(table, base, 1, n);
        Pad(' ', width - SizeNum(base, n) - (n == 0));
    } else {
        PutNum(table, base, width, n);
    }
}

static void PutSigned(signed int d, int left, int sign, int width) {
    if (d < 0) {
        Putc('-');
        PutBase(-d, lower, 10, left, 0, width-1);
    } else {
        PutBase(d, lower, 10, left, sign, width);
    }
}

/** Floating point printing **/
static void PutNormalFloat(float f, int left, int sign, int width, int prec) {
    if (prec < 0)
        prec = 6;
    
    if (f < 0) {
        Putc('-');
        width--;
        f = -f;
    }
    
    if (isnan(f)) {
        PutString("nan", left, width, 3);
    } else if (isinf(f)) {
        PutString("inf", left, width, 3);
    } else {
        int height;
        if (left) {
            height = 0;
            width -= SizeNum(10, (int)f) + 1;
        } else {
            height = width - (prec+1);
            width = 0;
        }
        
        PutBase((int)f, lower, 10, left, 0, height);    
        Putc('.');
        
        f = f - floorf(f);
        PutBase((int)(f*powf(10, prec)), lower, 10, 0, 0, prec);
        Pad(' ', width-prec);
    }
}

static void PutScienceFloat(float f, const char *table, int left, int sign, int width, int prec) {
    if (prec < 0)
        prec = 6;
    
    if (f < 0) {
        Putc('-');
        width--;
        f = -f;
    }
    
    if (f == 0) {
        PutString("0", left, width, 1);
    } else if (isnan(f)) {
        PutString("nan", left, width, 3);
    } else if (isinf(f)) {
        PutString("inf", left, width, 3);
    } else {
        float exp = floorf(log10f(f));
        float base = f / powf(10, exp);
        
        int height;
        if (left) {
            height = 0;
            width -= SizeNum(10, (int)f) + 1;
        } else {
            height = width - (prec + SizeNum(10, (int)exp) + 2);
            width = 0;
        }
        
        PutBase((int)f, lower, 10, left, 0, height);
        Putc('.');
        
        f = f - floorf(f);
        PutBase((int)(f*powf(10, prec)), lower, 10, 0, 0, prec);
        
        Putc(table[0xe]);
        PutBase((int)exp, lower, 10, 1, 1, width);
    }
}

        
// And finally printf itself
void Printf(const char *buffer, ...) {
    va_list args;
    int left, sign, width, prec;
    
    // Start the varargs processing.
    va_start(args, buffer);
    
    while (*buffer) {
        // Check for escape characters
next:   if (*buffer == '%') {
            buffer++;
    
            // Flags
            left = 0; 
            sign = 0;
            width = -1;
            prec = -1;
            
            while (true) {
                switch (*buffer++) {
                    // Flags
                    case '0': break;
                    case '-': left = 1; break;
                    case '+': sign = 1; break;
                    case ' ': sign = 2; break;
                    
                    case '1': 
                    case '2': 
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': buffer--; width = GetInt(&buffer); break;
                    
                    case '.': prec = GetInt(&buffer); break;
                    case '*': prec = va_arg(args, int); break;
                    
                    // Numbers
                    case 'i':
                    case 'd': PutSigned(va_arg(args, signed int), left, sign, width); goto next;
                    case 'u': PutBase(va_arg(args, unsigned int), lower, 10, left, sign, width); goto next;
                    case 'o': PutBase(va_arg(args, unsigned int), lower, 8,  left, sign, width); goto next;
                    case 'p':
                    case 'x': PutBase(va_arg(args, unsigned int), lower, 16, left, sign, width); goto next;
                    case 'X': PutBase(va_arg(args, unsigned int), upper, 16, left, sign, width); goto next;
                    
                    case 'f': 
                    case 'F': PutNormalFloat(va_d2f(args), left, sign, width, prec); goto next;
                    case 'e':
                    case 'g': PutScienceFloat(va_d2f(args), lower, left, sign, width, prec); goto next;
                    case 'E':
                    case 'G': PutScienceFloat(va_d2f(args), upper, left, sign, width, prec); goto next;
                    
                    // Other
                    case 's': PutString(va_arg(args, char *), left, width, prec); goto next;
                    case 'c': Putc((char)va_arg(args, int)); goto next;
                    default:  Putc('%'); goto next;
                }
            }
        } else {
            // Print out the next character
            if (*buffer == '\n')
                Putc('\r');
            
            Putc(*buffer++);
        }
    }
    
    va_end(args);
}



