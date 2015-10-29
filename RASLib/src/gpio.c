//*****************************************************************************
//
// gpio - GPIO related funcions
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

#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

#include <StellarisWare/inc/hw_ints.h>
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_gpio.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/driverlib/gpio.h>
#include <StellarisWare/driverlib/sysctl.h>


// Port table is used externally to lookup port values
const unsigned long PIN_PORTS[PORT_COUNT] = {
    GPIO_PORTA_BASE,
    GPIO_PORTB_BASE,
    GPIO_PORTC_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTF_BASE
};

volatile uint32_t *PIN_BIT_ADDR[PIN_COUNT] = {
  (volatile uint32_t *) NULL, // PA0 uart 0
  (volatile uint32_t *) NULL, // PA1
  ((volatile uint32_t *) GPIO_PORTA_BASE + GPIO_O_DATA) + (1 << 2), // PA2
  ((volatile uint32_t *) GPIO_PORTA_BASE + GPIO_O_DATA) + (1 << 3), // PA3
  ((volatile uint32_t *) GPIO_PORTA_BASE + GPIO_O_DATA) + (1 << 4), // PA4
  ((volatile uint32_t *) GPIO_PORTA_BASE + GPIO_O_DATA) + (1 << 5), // PA5
  ((volatile uint32_t *) GPIO_PORTA_BASE + GPIO_O_DATA) + (1 << 6), // PA6
  ((volatile uint32_t *) GPIO_PORTA_BASE + GPIO_O_DATA) + (1 << 7), // PA7

  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 0), // PB0
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 1), // PB1
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 2), // PB2
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 3), // PB3
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 4), // PB4
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 5), // PB5
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 6), // PB6
  ((volatile uint32_t *) GPIO_PORTB_BASE + GPIO_O_DATA) + (1 << 7), // PB7

  (volatile uint32_t *) NULL, // PC0 JTAG
  (volatile uint32_t *) NULL, // PC1
  (volatile uint32_t *) NULL, // PC2
  (volatile uint32_t *) NULL, // PC3
  ((volatile uint32_t *) GPIO_PORTC_BASE + GPIO_O_DATA) + (1 << 4), // PC4
  ((volatile uint32_t *) GPIO_PORTC_BASE + GPIO_O_DATA) + (1 << 5), // PC5
  ((volatile uint32_t *) GPIO_PORTC_BASE + GPIO_O_DATA) + (1 << 6), // PC6
  ((volatile uint32_t *) GPIO_PORTC_BASE + GPIO_O_DATA) + (1 << 7), // PC7

  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 0), // PD0
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 1), // PD1
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 2), // PD2
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 3), // PD3
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 4), // PD4
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 5), // PD5
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 6), // PD6
  ((volatile uint32_t *) GPIO_PORTD_BASE + GPIO_O_DATA) + (1 << 7), // PD7
  
  ((volatile uint32_t *) GPIO_PORTE_BASE + GPIO_O_DATA) + (1 << 0), // PE0
  ((volatile uint32_t *) GPIO_PORTE_BASE + GPIO_O_DATA) + (1 << 1), // PE1
  ((volatile uint32_t *) GPIO_PORTE_BASE + GPIO_O_DATA) + (1 << 2), // PE2
  ((volatile uint32_t *) GPIO_PORTE_BASE + GPIO_O_DATA) + (1 << 3), // PE3
  ((volatile uint32_t *) GPIO_PORTE_BASE + GPIO_O_DATA) + (1 << 4), // PE4
  ((volatile uint32_t *) GPIO_PORTE_BASE + GPIO_O_DATA) + (1 << 5), // PE5
  (volatile uint32_t *) NULL, // PE6 these pins d(on't e)xist
  (volatile uint32_t *) NULL, // PE7
  
  ((volatile uint32_t *) GPIO_PORTF_BASE + GPIO_O_DATA) + (1 << 0), // PF0
  ((volatile uint32_t *) GPIO_PORTF_BASE + GPIO_O_DATA) + (1 << 1), // PF1
  ((volatile uint32_t *) GPIO_PORTF_BASE + GPIO_O_DATA) + (1 << 2), // PF2
  ((volatile uint32_t *) GPIO_PORTF_BASE + GPIO_O_DATA) + (1 << 3), // PF3
  ((volatile uint32_t *) GPIO_PORTF_BASE + GPIO_O_DATA) + (1 << 4), // PF4
  (volatile uint32_t *) NULL, // PF5 these pins don't exist
  (volatile uint32_t *) NULL, // PF6
  (volatile uint32_t *) NULL, // PF7
};

// Internally used struct containing interrupt data
typedef struct {
    tCallback callback;
    void *data;
} tPinTask;

// Array of all possible pin interrupts
tPinTask pinTaskBuffer[PIN_COUNT];

// Internally used macro for defining port interrupt handlers
#define PORT_HANDLER(PORT)                                      \
void Port##PORT##Handler(void) {                                \
    unsigned long i, status;                                    \
    status = GPIOPinIntStatus(GPIO_PORT##PORT##_BASE, true);    \
                                                                \
    for (i = 0; i < 8; i++){                                    \
        if (status & (0x1 << i)) {                              \
            tPinTask *task = &pinTaskBuffer[PIN_##PORT##0 | i]; \
            task->callback(task->data);                         \
        }                                                       \
    }                                                           \
    GPIOPinIntClear(GPIO_PORT##PORT##_BASE, status);            \
}

// Interrupt handlers for Ports A through F
PORT_HANDLER(A);
PORT_HANDLER(B);
PORT_HANDLER(C);
PORT_HANDLER(D);
PORT_HANDLER(E);
PORT_HANDLER(F);

// Initialize GPIO
void InitializeGPIO(void) {
    int i;
    
    // Just clear all handlers
    for (i=0; i<PIN_COUNT; i++)
        pinTaskBuffer[i].callback = Dummy;
    
    // Enable the peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    // Special workarounds for PF0 and PD7
    // For more info lookup NMI mux issue on the LM4F
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
    
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
        
    // Enable the interrupts
    // This shouldn't be nescessary but for some reason is...
    IntEnable(INT_GPIOA);
    IntEnable(INT_GPIOB);
    IntEnable(INT_GPIOC);
    IntEnable(INT_GPIOD);
    IntEnable(INT_GPIOE);
    IntEnable(INT_GPIOF);
}

// Get Pin value as a boolean
tBoolean GetPin(tPin pin) {
    // Setting pin direction is just a bit set and fairly trivial
    GPIOPinTypeGPIOInput(PORT_VAL(pin), PIN_VAL(pin));
    
    // Get the actual pin value
    return (*PIN_BIT_ADDR[pin] != 0);
}

// Set Pin value as a boolean
void SetPin(tPin pin, tBoolean val) {
    // Setting pin direction is just a bit set and fairly trivial
    GPIOPinTypeGPIOOutput(PORT_VAL(pin), PIN_VAL(pin));
    
    // Set the actual pin value
    *(PIN_BIT_ADDR[pin]) = val ? 0xff : 0x00;
}

// Set a pin into high impedance mode
void SetPinZ(tPin pin) {
    // Setting pin direction to input places it in high impedance mode
    GPIOPinTypeGPIOInput(PORT_VAL(pin), PIN_VAL(pin));
}

// Add a weak pull up resistor to the pin
void PullUpPin(tPin pin) {
    GPIOPadConfigSet(PORT_VAL(pin), PIN_VAL(pin), 
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

// Add a weak pull down resistor to the pin
void PullDownPin(tPin pin) {
    GPIOPadConfigSet(PORT_VAL(pin), PIN_VAL(pin), 
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
}

// Internally used function to register a pin interrupt
static void CallOnPinType(tCallback callback, void *data, tPin pin, unsigned long type) {
    tPinTask *task = &pinTaskBuffer[pin];

    // Stop the interrupt first to avoid a race condition
    GPIOPinIntDisable(PORT_VAL(pin), PIN_VAL(pin));
    task->callback = Dummy;
    
    // Make sure the pin is setup as an input
    GPIOPinTypeGPIOInput(PORT_VAL(pin), PIN_VAL(pin));
    
    // If a null pointer is passed then we just leave the Dummy
    // to unregister the callback
    if (callback) {
        task->data = data;
        task->callback = callback;
        
        // Setup the interrupts
        GPIOIntTypeSet(PORT_VAL(pin), PIN_VAL(pin), type);
        GPIOPinIntClear(PORT_VAL(pin), PIN_VAL(pin));
        GPIOPinIntEnable(PORT_VAL(pin), PIN_VAL(pin));
    }
}

// Register a callback to be called when the pin's value changes, 
// the state of the pin can then be determined through the GetPin function.
void CallOnPin(tCallback callback, void *data, tPin pin) {
    CallOnPinType(callback, data, pin, GPIO_BOTH_EDGES);
}

// Register a callback to be called when the pin's value goes from low to high, 
// the state of the pin can then be determined through the GetPin function.
void CallOnPinRising(tCallback callback, void *data, tPin pin) {
    CallOnPinType(callback, data, pin, GPIO_RISING_EDGE);
}

// Register a callback to be called when the pin's value goes from high to low, 
// the state of the pin can then be determined through the GetPin function.
void CallOnPinFalling(tCallback callback, void *data, tPin pin) {
    CallOnPinType(callback, data, pin, GPIO_FALLING_EDGE);
}
