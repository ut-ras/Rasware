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
#include <stdint.h>

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

static tBoolean hack_offset;

struct tUART {
        const uint32_t base;
        tBoolean echo;
        char ungotten;
};

static struct tUART uartModules[UART_COUNT] = {
        {UART0_BASE},
        {UART1_BASE},
        {UART2_BASE},
        {UART3_BASE},
        {UART4_BASE},
        {UART5_BASE},
        {UART6_BASE},
        {UART7_BASE}
};

// Sets up a simple console through UART0
tUART * InitializeUARTModule(int module, int baud) {
        tUART * toRet = &uartModules[module];
        switch (module) {
        case 0:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
                GPIOPinConfigure(GPIO_PA0_U0RX);
                GPIOPinConfigure(GPIO_PA1_U0TX);
                GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
                break;
        case 1:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
                GPIOPinConfigure(GPIO_PB0_U1RX);
                GPIOPinConfigure(GPIO_PB1_U1TX);
                GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
                break;
        case 2:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
                GPIOPinConfigure(GPIO_PD6_U2RX);
                GPIOPinConfigure(GPIO_PD7_U2TX);
                GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
                break;
        case 3:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
                GPIOPinConfigure(GPIO_PC6_U3RX);
                GPIOPinConfigure(GPIO_PC7_U3TX);
                GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
                break;
        case 4:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
                GPIOPinConfigure(GPIO_PC4_U4RX);
                GPIOPinConfigure(GPIO_PC5_U4TX);
                GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
                break;
        case 5:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
                GPIOPinConfigure(GPIO_PE4_U5RX);
                GPIOPinConfigure(GPIO_PE5_U5TX);
                GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
                break;
        case 6:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
                GPIOPinConfigure(GPIO_PD4_U6RX);
                GPIOPinConfigure(GPIO_PD5_U6TX);
                GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
                break;
        case 7:
                SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
                GPIOPinConfigure(GPIO_PE0_U7RX);
                GPIOPinConfigure(GPIO_PE1_U7TX);
                GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
                break;
        }
// Initialize the UART for console I/O.
        UARTConfigSetExpClk(toRet->base, SysCtlClockGet(), baud,
                            (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_WLEN_8));

// Default to assuming the offset makes sense
        return toRet;
}

void InitializeUART(int baud) {
        InitializeUARTModule(0, baud);
        hack_offset = false;
}

static tBoolean DoubleTest(int, ...);
void InitializeDoublePrintHack(void) {
    // Just try the offset and see if its wrong
    hack_offset ^= !DoubleTest(1, 1.0f);
}

void fPutc(tUART * module, char ch) {
        UARTCharPut(module->base, ch);
}

void Putc(char ch) {
        fPutc(&uartModules[0], ch);
}

// Busy-waits for an input character and then returns it
unsigned char fGetc(tUART * module) {
  unsigned char tmp;
  if( module->ungotten == 0) {
    tmp =  UARTCharGet(module->base);
    if (module->echo == true) fPutc(module, tmp);
    return tmp;
  } else {
    tmp = module->ungotten;
    module->ungotten = 0;
    return tmp;
  }
}

unsigned char Getc(void) {
        return fGetc(&uartModules[0]);
}

void SetEcho (tUART * module, unsigned char on) {
  module->echo = on;
}

void unGetC (tUART * module, unsigned char unGetThis) {
  module->ungotten = unGetThis;
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

unsigned int GetAToI (tUART * module) {
        unsigned char tmp;
        unsigned int toRet = 0;
        unsigned char negativeP = false;
        if ((tmp = fGetc(module)) == '-')
                negativeP = true;
        else if (tmp != '+')
                unGetC(module, tmp);
        while (matchCharP( (tmp = fGetc(module)), "0123456789", 10))
                toRet = (toRet * 10) + (tmp - '0');
        unGetC(module, tmp);
        if (negativeP) toRet = -toRet;
        return toRet;
}

float GetAToF (tUART * module) {
        unsigned char tmp;
        unsigned int num = 0;
        unsigned int size = 1;
        while (matchCharP( (tmp = fGetc(module)), "0123456789", 10)){
                num = (num * 10) + (tmp - '0');
                size *= 10;
        }
        unGetC(module, tmp);
        return ((float) num) / size;
}

unsigned int GetOToI (tUART * module) {
        unsigned char tmp;
        unsigned int toRet = 0;
        unsigned char negativeP = false;
        if ((tmp = fGetc(module)) == '-')
                negativeP = true;
        else if (tmp != '+')
                unGetC(module, tmp);
        while (matchCharP( (tmp = fGetc(module)), "01234567", 8))
                toRet = (toRet * 8) + (tmp - '0');
        unGetC(module, tmp);
        if (negativeP) toRet = -toRet;
        return toRet;
}

unsigned int GetXToI (tUART * module) {
        unsigned char tmp;
        unsigned int toRet = 0;
        unsigned char negativeP = false;
        if ((tmp = fGetc(module)) == '-')
                negativeP = true;
        else if (tmp != '+')
                unGetC(module, tmp);
        while (matchCharP( (tmp = fGetc(module)), "0123456789abcdefABCDEF", 22))
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
        unGetC(module, tmp);
        if (negativeP) toRet = -toRet;
        return toRet;
}

unsigned int vScanf(tUART * module, const char * formatString, va_list ap ) {
        int ret = 0;
        int i = -1;
        unsigned char tmp;
        int length;
        char * s_ptr;
        const char * braket_ptr;
        unsigned int braket_len;
        unsigned int * i_ptr;
        float * f_ptr;
        while (formatString[++i] != '\0') {
                switch (formatString[i]) {
                case ' ':
                case '\t':
                        while (whiteSpaceP( (tmp = fGetc(module)) ));
                        unGetC(module, tmp);
                        break;
                case '%':
                        length = 0;
                fmtTop:
                        switch (formatString[++i]) {
                        case '%' :
                                if (fGetc(module) == '%') break;
                                else goto exit;
                        case 'c' :
                                *(va_arg(ap, char *)) = fGetc(module);
                                break;
                        case 's' :
                                if (length == 0) length =  (1<<30);
                                s_ptr = va_arg(ap, char*);
                                while ( ! whiteSpaceP( (*(s_ptr++) = fGetc(module)) ) && length-- > 0);
                                unGetC(module, s_ptr[-1]);
                                s_ptr[-1] = '\0';
                                break;
                        case '[':
                                if (length == 0) length =  (1<<30);
                                braket_ptr = &formatString[i+1];
                                s_ptr = va_arg(ap, char*);
                                while (formatString[++i] != ']' && formatString[i] != '\0');
                                braket_len = &formatString[i] - braket_ptr;
                                if  (*braket_ptr == '^') {
                                        braket_len--;
                                        braket_ptr++;
                                        while (!matchCharP( (*(s_ptr++) = fGetc(module)), braket_ptr, braket_len) && length-- > 0);
                                }
                                else
                                        while (matchCharP( (*(s_ptr++) = fGetc(module)), braket_ptr, braket_len) && length-- > 0);
                                unGetC(module, s_ptr[-1]);
                                s_ptr[-1] = '\0';
                                break;
                        case 'i':
                                tmp = fGetc(module);
                                if (tmp == '0') {
                                        tmp = fGetc(module);
                                        if (tmp == 'x')
                                                goto hex;
                                        else {
                                                unGetC(module, tmp);
                                                goto octal; } }
                                else
                                        goto decimal;
                        case 'o':
                        octal:
                                i_ptr = va_arg(ap, unsigned int *);
                                *i_ptr = GetOToI(module);
                                break;
                        case 'd':
                        case 'u':
                        decimal:
                                i_ptr = va_arg(ap, unsigned int *);
                                *i_ptr = GetAToI(module);
                                break;
                        case 'x':
                        hex:
                                i_ptr = va_arg(ap, unsigned int *);
                                *i_ptr = GetXToI(module);
                                break;
                        case 'f':
                        case 'e':
                        case 'g':
                        case 'a':
                                f_ptr = va_arg(ap, float *);
                                *f_ptr = (float) GetAToI(module);
                                tmp = fGetc(module);
                                if ( tmp == '.' ) {
                                        *f_ptr += GetAToF(module);
                                        tmp = fGetc(module);
                                }
                                if ( tmp == 'E' || tmp == 'e' ) {
                                        *f_ptr *= (float) pow(10, GetAToI(module));
                                        tmp = fGetc(module);
                                }
                                unGetC(module,tmp);
                                break;
                        case '0': case '1': case '2':
                        case '5': case '4': case '3':
                        case '6': case '7': case '8':
                        case '9':
                                length = (length * 10) + (formatString[i] - '0');
                                goto fmtTop;
                        default :
                                goto exit;
                        }
                        ret++;
                        break;
                default:
                        tmp = fGetc(module);
                        if (tmp != formatString[i] ) goto exit;
                }
        }
exit:
        return ret;
}

unsigned int Scanf(const char * formatString, ...) {
        va_list ap;
        va_start(ap, formatString);
        vScanf(&uartModules[0], formatString, ap);
        va_end(ap);
}

unsigned int fScanf(tUART * module, const char * formatString, ...) {
        va_list ap;
        va_start(ap, formatString);
        vScanf(module, formatString, ap);
        va_end(ap);
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

// Outputs the characters of a null-terminated string over UART
int fPuts(tUART * module, const char *buffer, int count) {
    int i;
    
    // Loop while there are more characters to send.
    for (i = 0; i < count; i++) {
        // Handle new lines
        if (buffer[i] == '\n') {
            UARTCharPut(module->base, '\r');
        } else if (buffer[i] == '\0') {
            return i;
        }
        
        UARTCharPut(module->base, buffer[i]);
    }
    
    return count;
}

int Puts(const char *buffer, int count) {
        fPuts(&uartModules[0], buffer, count);
}

// Checks to see if a character has been received in the UART buffer
int KeyWasPressed(void) {
    return (UARTCharsAvail(UART0_BASE) > 0);
}

int fKeyWasPressed(tUART * module) {
        return (UARTCharsAvail(module->base) > 0);
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

    if ((((unsigned int)(*args) & 0x4) == 0x4) ^ hack_offset) {
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

static tBoolean DoubleTest(int eh, ...) {
    float test;
    va_list args;
    va_start(args, eh);
    test = va_d2f(args);
    va_end(args);
    
    return eh == (int)test;
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

static void Pad(tUART * module, char c, int n) {
    int i;
    
    for (i = 0; i < n; i++) {
            fPutc(module, c);
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

static void PutString(tUART * module, const char *buffer, int left, int width, int prec) {
    int len;
    
    if (prec < 0) prec = 0x7fffffff;
    
    len = SizeString(buffer, prec);
    
    if (len > width) {
            fPuts(module, buffer, prec);
    } else if (left) {
            fPuts(module, buffer, prec);
            Pad(module, ' ', width - len);
    } else {
            Pad(module, ' ', width - len);
            fPuts(module, buffer, prec);
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

static void PutNum(tUART * module, const char *table, int base, int count, unsigned int n) {
    if (!n && count <= 0)
        return;
    
    PutNum(module, table, base, count-1, n/base);
    fPutc(module, table[n%base]);
}

static void PutBase(tUART * module, unsigned int n, const char *table, int base, int left, int sign, int width) {
    if (sign == 1) {
            fPutc(module, '+');
        width--;
    } else if (sign == 2) {
            fPutc(module, ' ');
        width--;
    }
    
    if (width < 1) 
        width = 1;
    
    if (left) {
            PutNum(module, table, base, 1, n);
            Pad(module, ' ', width - SizeNum(base, n) - (n == 0));
    } else {
            PutNum(module, table, base, width, n);
    }
}

static void PutSigned(tUART *module, signed int d, int left, int sign, int width) {
    if (d < 0) {
            fPutc(module, '-');
            PutBase(module, -d, lower, 10, left, 0, width-1);
    } else {
            PutBase(module, d, lower, 10, left, sign, width);
    }
}

/** Floating point printing **/
static void PutNormalFloat(tUART * module, float f, int left, int sign, int width, int prec) {
    if (prec < 0)
        prec = 6;
    
    if (f < 0) {
            fPutc(module, '-');
        width--;
        f = -f;
    }
    
    if (isnan(f)) {
            PutString(module, "nan", left, width, 3);
    } else if (isinf(f)) {
            PutString(module, "inf", left, width, 3);
    } else {
        int height;
        if (left) {
            height = 0;
            width -= SizeNum(10, (int)f) + 1;
        } else {
            height = width - (prec+1);
            width = 0;
        }
        
        PutBase(module, (int)f, lower, 10, left, 0, height);    
        fPutc(module, '.');
        
        f = f - floorf(f);
        PutBase(module, (int)(f*powf(10, prec)), lower, 10, 0, 0, prec);
        Pad(module, ' ', width-prec);
    }
}

static void PutScienceFloat(tUART * module, float f, const char *table, int left, int sign, int width, int prec) {
    if (prec < 0)
        prec = 6;
    
    if (f < 0) {
            fPutc(module, '-');
        width--;
        f = -f;
    }
    
    if (f == 0) {
            PutString(module, "0", left, width, 1);
    } else if (isnan(f)) {
            PutString(module, "nan", left, width, 3);
    } else if (isinf(f)) {
            PutString(module, "inf", left, width, 3);
    } else {
        float exp = floorf(log10f(f));
        
        int height;
        if (left) {
            height = 0;
            width -= SizeNum(10, (int)f) + 1;
        } else {
            height = width - (prec + SizeNum(10, (int)exp) + 2);
            width = 0;
        }
        
        PutBase(module, (int)f, lower, 10, left, 0, height);
        fPutc(module, '.');
        
        f = f - floorf(f);
        PutBase(module, (int)(f*powf(10, prec)), lower, 10, 0, 0, prec);
        
        fPutc(module, table[0xe]);
        PutBase(module, (int)exp, lower, 10, 1, 1, width);
    }
}

        
// And finally printf itself
void vPrintf(tUART * module, const char *buffer, va_list args) {
    int left, sign, width, prec;
    
    // Start the varargs processing.
    
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
                case 'd': PutSigned(module, va_arg(args, signed int), left, sign, width); goto next;
                case 'u': PutBase(module, va_arg(args, unsigned int), lower, 10, left, sign, width); goto next;
                case 'o': PutBase(module, va_arg(args, unsigned int), lower, 8,  left, sign, width); goto next;
                case 'p':
                case 'x': PutBase(module, va_arg(args, unsigned int), lower, 16, left, sign, width); goto next;
                case 'X': PutBase(module, va_arg(args, unsigned int), upper, 16, left, sign, width); goto next;

                case 'f':
                case 'F': PutNormalFloat(module, va_d2f(args), left, sign, width, prec); goto next;
                case 'e':
                case 'g': PutScienceFloat(module, va_d2f(args), lower, left, sign, width, prec); goto next;
                case 'E':
                case 'G': PutScienceFloat(module, va_d2f(args), upper, left, sign, width, prec); goto next;

// Other
                case 's': PutString(module, va_arg(args, char *), left, width, prec); goto next;
                case 'c': fPutc(module, (char)va_arg(args, int)); goto next;
                default:  fPutc(module, '%'); goto next;
                }
            }
        } else {
            // Print out the next character
            if (*buffer == '\n')
                    fPutc(module, '\r');
            
            fPutc(module, *buffer++);
        }
    }
    
}

void Printf(const char *buffer, ...) {
        va_list args;
        va_start(args, buffer);
        vPrintf(&uartModules[0], buffer, args);
        va_end(args);
}

void fPrintf(tUART * module, const char *buffer, ...) {
        va_list args;
        va_start(args, buffer);
        vPrintf(module, buffer, args);
        va_end(args);
}
