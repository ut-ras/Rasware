//*****************************************************************************
//
// Startup.c - Startup code for use on RASLib supported compilers
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

#include <StellarisWare/inc/hw_nvic.h>
#include <StellarisWare/inc/hw_types.h>

// Initialization entry point for RASLib
extern void InitializeMCU(void);

// The entry point for the application
extern int main(void);

// Forward declaration of the default fault handlers
void ResetHandler(void);
void NmiHandler(void);
void FaultHandler(void);
void IntDefaultHandler(void);

// Declaration of interrupt service routines
extern void PanicHandler(void);
extern void ADC0SS0Handler(void);
extern void ADC0SS1Handler(void);
extern void ADC1SS0Handler(void);
extern void ADC1SS1Handler(void);
extern void PortAHandler(void);
extern void PortBHandler(void);
extern void PortCHandler(void);
extern void PortDHandler(void);
extern void PortEHandler(void);
extern void PortFHandler(void);
extern void SysTickHandler(void);
extern void WTimer0AHandler(void);
extern void WTimer0BHandler(void);
extern void WTimer1AHandler(void);
extern void WTimer1BHandler(void);
extern void WTimer2AHandler(void);
extern void WTimer2BHandler(void);
extern void WTimer3AHandler(void);
extern void WTimer3BHandler(void);
extern void WTimer4AHandler(void);
extern void WTimer4BHandler(void);
extern void WTimer5Handler(void);
extern void I2C0Handler(void);
extern void I2C1Handler(void);
extern void I2C2Handler(void);
extern void I2C3Handler(void);
extern void I2C4Handler(void);
extern void I2C5Handler(void);


// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x00000000
__attribute__((section(".isr_vector")))
void (* const __Vectors[])(void) = {
    (void (*)())(0x20008000),         // The initial stack pointer
    ResetHandler,                           // The reset handler
    NmiHandler,                             // The NMI handler
    FaultHandler,                           // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    IntDefaultHandler,                      // The PendSV handler
    SysTickHandler,                         // The SysTick handler
    PortAHandler,                           // GPIO Port A
    PortBHandler,                           // GPIO Port B
    PortCHandler,                           // GPIO Port C
    PortDHandler,                           // GPIO Port D
    PortEHandler,                           // GPIO Port E
    IntDefaultHandler,                      // UART0 Rx and Tx
    IntDefaultHandler,                      // UART1 Rx and Tx
    IntDefaultHandler,                      // SSI0 Rx and Tx
    I2C0Handler,                            // I2C0 Master and Slave
    IntDefaultHandler,                      // PWM Fault
    IntDefaultHandler,                      // PWM Generator 0
    IntDefaultHandler,                      // PWM Generator 1
    IntDefaultHandler,                      // PWM Generator 2
    IntDefaultHandler,                      // Quadrature Encoder 0
    ADC0SS0Handler,                         // ADC Sequence 0
    ADC0SS1Handler,                         // ADC Sequence 1
    IntDefaultHandler,                      // ADC Sequence 2
    IntDefaultHandler,                      // ADC Sequence 3
    IntDefaultHandler,                      // Watchdog timer
    IntDefaultHandler,                      // Timer 0 subtimer A
    IntDefaultHandler,                      // Timer 0 subtimer B
    IntDefaultHandler,                      // Timer 1 subtimer A
    IntDefaultHandler,                      // Timer 1 subtimer B
    IntDefaultHandler,                      // Timer 2 subtimer A
    IntDefaultHandler,                      // Timer 2 subtimer B
    IntDefaultHandler,                      // Analog Comparator 0
    IntDefaultHandler,                      // Analog Comparator 1
    IntDefaultHandler,                      // Analog Comparator 2
    IntDefaultHandler,                      // System Control (PLL, OSC, BO)
    IntDefaultHandler,                      // FLASH Control
    PortFHandler,                           // GPIO Port F
    IntDefaultHandler,                      // GPIO Port G
    IntDefaultHandler,                      // GPIO Port H
    IntDefaultHandler,                      // UART2 Rx and Tx
    IntDefaultHandler,                      // SSI1 Rx and Tx
    IntDefaultHandler,                      // Timer 3 subtimer A
    IntDefaultHandler,                      // Timer 3 subtimer B
    I2C1Handler,                            // I2C1 Master and Slave
    IntDefaultHandler,                      // Quadrature Encoder 1
    IntDefaultHandler,                      // CAN0
    IntDefaultHandler,                      // CAN1
    IntDefaultHandler,                      // CAN2
    IntDefaultHandler,                      // Ethernet
    IntDefaultHandler,                      // Hibernate
    IntDefaultHandler,                      // USB0
    IntDefaultHandler,                      // PWM Generator 3
    IntDefaultHandler,                      // uDMA Software Transfer
    IntDefaultHandler,                      // uDMA Error
    ADC1SS0Handler,                         // ADC1 Sequence 0
    ADC1SS1Handler,                         // ADC1 Sequence 1
    IntDefaultHandler,                      // ADC1 Sequence 2
    IntDefaultHandler,                      // ADC1 Sequence 3
    IntDefaultHandler,                      // I2S0
    IntDefaultHandler,                      // External Bus Interface 0
    IntDefaultHandler,                      // GPIO Port J
    IntDefaultHandler,                      // GPIO Port K
    IntDefaultHandler,                      // GPIO Port L
    IntDefaultHandler,                      // SSI2 Rx and Tx
    IntDefaultHandler,                      // SSI3 Rx and Tx
    IntDefaultHandler,                      // UART3 Rx and Tx
    IntDefaultHandler,                      // UART4 Rx and Tx
    IntDefaultHandler,                      // UART5 Rx and Tx
    IntDefaultHandler,                      // UART6 Rx and Tx
    IntDefaultHandler,                      // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    I2C2Handler,                            // I2C2 Master and Slave
    I2C3Handler,                            // I2C3 Master and Slave
    IntDefaultHandler,                      // Timer 4 subtimer A
    IntDefaultHandler,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // Timer 5 subtimer A
    IntDefaultHandler,                      // Timer 5 subtimer B
    WTimer0AHandler,                        // Wide Timer 0 subtimer A
    WTimer0BHandler,                        // Wide Timer 0 subtimer B
    WTimer1AHandler,                        // Wide Timer 1 subtimer A
    WTimer1BHandler,                        // Wide Timer 1 subtimer B
    WTimer2AHandler,                        // Wide Timer 2 subtimer A
    WTimer2BHandler,                        // Wide Timer 2 subtimer B
    WTimer3AHandler,                        // Wide Timer 3 subtimer A
    WTimer3BHandler,                        // Wide Timer 3 subtimer B
    WTimer4AHandler,                        // Wide Timer 4 subtimer A
    WTimer4BHandler,                        // Wide Timer 4 subtimer B
    WTimer5Handler,                         // Wide Timer 5 subtimer A
    IntDefaultHandler,                      // Wide Timer 5 subtimer B
    IntDefaultHandler,                      // FPU
    IntDefaultHandler,                      // PECI 0
    IntDefaultHandler,                      // LPC 0
    I2C4Handler,                            // I2C4 Master and Slave
    I2C5Handler,                            // I2C5 Master and Slave
    IntDefaultHandler,                      // GPIO Port M
    IntDefaultHandler,                      // GPIO Port N
    IntDefaultHandler,                      // Quadrature Encoder 2
    IntDefaultHandler,                      // Fan 0
    0,                                      // Reserved
    IntDefaultHandler,                      // GPIO Port P (Summary or P0)
    IntDefaultHandler,                      // GPIO Port P1
    IntDefaultHandler,                      // GPIO Port P2
    IntDefaultHandler,                      // GPIO Port P3
    IntDefaultHandler,                      // GPIO Port P4
    IntDefaultHandler,                      // GPIO Port P5
    IntDefaultHandler,                      // GPIO Port P6
    IntDefaultHandler,                      // GPIO Port P7
    IntDefaultHandler,                      // GPIO Port Q (Summary or Q0)
    IntDefaultHandler,                      // GPIO Port Q1
    IntDefaultHandler,                      // GPIO Port Q2
    IntDefaultHandler,                      // GPIO Port Q3
    IntDefaultHandler,                      // GPIO Port Q4
    IntDefaultHandler,                      // GPIO Port Q5
    IntDefaultHandler,                      // GPIO Port Q6
    IntDefaultHandler,                      // GPIO Port Q7
    IntDefaultHandler,                      // GPIO Port R
    IntDefaultHandler,                      // GPIO Port S
    IntDefaultHandler,                      // PWM 1 Generator 0
    IntDefaultHandler,                      // PWM 1 Generator 1
    IntDefaultHandler,                      // PWM 1 Generator 2
    IntDefaultHandler,                      // PWM 1 Generator 3
    IntDefaultHandler                       // PWM 1 Fault
};


// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment and 
// the RO and RW memory is garunteed to be sequential.
// The actual definitions are linker dependent and use the preprocessor to select.
#ifdef __CC_ARM
    extern unsigned long Load$$ER_RW$$RW$$Base;
    extern unsigned long Image$$ER_RW$$RW$$Base;
    extern unsigned long Image$$ER_RW$$RW$$Limit;
    extern unsigned long Image$$ER_ZI$$ZI$$Base;
    extern unsigned long Image$$ER_ZI$$ZI$$Limit;

    #define DATA_LOAD   &(Load$$ER_RW$$RW$$Base)
    #define DATA_START  &(Image$$ER_RW$$RW$$Base)
    #define DATA_END    &(Image$$ER_RW$$RW$$Limit)
    #define BSS_START   &(Image$$ER_ZI$$ZI$$Base)
    #define BSS_END     &(Image$$ER_ZI$$ZI$$Limit)
#else
    extern unsigned long _sidata;
    extern unsigned long _data;
    extern unsigned long _edata;
    extern unsigned long _bss;
    extern unsigned long _ebss;

    #define DATA_LOAD   &_sidata
    #define DATA_START  &_data
    #define DATA_END    &_edata
    #define BSS_START   &_bss
    #define BSS_END     &_ebss
#endif


// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
void ResetHandler(void) {
    register unsigned long *src;
    register unsigned long *dest;

    // Copy the data segment initializers from flash to SRAM.
    src = DATA_LOAD;
    dest = DATA_START;
    while (dest < DATA_END) {
        *dest++ = *src++;
    }

    // Zero fill the bss segment.
    dest = BSS_START;
    while (dest < BSS_END) {
        *dest++ = 0;
    }

    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);
    
    // Initialize RASLib
    InitializeMCU();

    // Call the application's entry point.
    main();

    // Enter an infinite loop if we leave main
    while (1) {}
}

// This is the code that gets called when the processor receives a NMI.  This
// must pass through to avoid a race condition during startup
void NmiHandler(void) {
    // Pass through
}

// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
void FaultHandler(void) {
    // Panic
    PanicHandler();
    // Enter an infinite loop
    while (1) {}
}

// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
void IntDefaultHandler(void) {
    // Panic
    PanicHandler();
    // Go into an infinite loop
    while (1) {}
}
