//*****************************************************************************
//
// adc - Analog to Digital converter drivers
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

#include "adc.h"

#include <StellarisWare/inc/hw_ints.h>
#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/interrupt.h>
#include <StellarisWare/driverlib/adc.h>
#include <StellarisWare/driverlib/gpio.h>


// Definition of ADC Pin-map for the TM4C1233H6PM / LM4F120H5QR/
// ADC#  | Pin Assignement
// ------+-----------------
// AIN0  |  PIN_E3 
// AIN1  |  PIN_E2 
// AIN2  |  PIN_E1
// AIN3  |  PIN_E0
// AIN4  |  PIN_D3
// AIN5  |  PIN_D2
// AIN6  |  PIN_D1
// AIN7  |  PIN_D0
// AIN8  |  PIN_E5
// AIN9  |  PIN_E4
// AIN10 |  PIN_B4
// AIN11 |  PIN_B5

// Resolution is a property of the microcontroller
// and is 12 bits for the LM4F
#define ADC_RESOLUTION 12
#define ADC_MAX (1<<ADC_RESOLUTION)

// The ADC read time is also per microcontroller
// Yes, the ADC in the LM4F takes a single microsecond
// Oversampling will slow this down and is taken into account below
#define ADC_TIME 1

// The number of ADC modules also varies per microcontroller,
// this program takes care to spread the created ADCs accross
// the available modules, however special care must be take 
// to make sure the ADC / Module ratio does not exceed 8
// which is the maximum ADCs the first sequence can hold
#define ADC_MODULE_COUNT 2


// Internally used struct representing
// a single ADC module
typedef struct ADCModule {
    // Constant StellarisWare values 
    // specific to each module
    const unsigned long BASE;
    const unsigned long PERIPH;
    const unsigned long INT;
    
    // Timing related information
    int id;
    tTime period;
    
    // The ADCs being read continously are
    // stored in this linked list
    tADC *contQueue;
    
    // The ADCs being read once are stored 
    // in this linked list, with the end pointed
    // to for efficiency
    tADC *singleQueue;
    tADC **singleEnd;
    
    // A simple flag for initialization
    tBoolean initialized;
} tADCModule;


// Array of available modules
static tADCModule modBuffer[ADC_MODULE_COUNT] = {
    {ADC0_BASE, SYSCTL_PERIPH_ADC0, INT_ADC0SS0},
    {ADC1_BASE, SYSCTL_PERIPH_ADC1, INT_ADC1SS0},
};


// Definition of struct ADC
// Defined to tADC in adc.h
struct ADC {
    // Pointer to the module its using
    tADCModule *module;
    
    // The most recent accumulated value
    unsigned long value;
    
    // The pin we're reading from
    tPin pin;
    
    // Callback data
    tCallback callback;
    void *data;
    
    // The next ADC in linked lists
    tADC *next;
    
    // Some state variables
    tBoolean in_callback : 1;
    tBoolean continous : 1;
    volatile tBoolean pending : 1;
};

// Buffer of adc structs to use
static tADC adcBuffer[ADC_COUNT];

static int adcCount = 0;


// A lookup table for channel values based on pins
// unfortunately ADC_CTL_CH0 == 0 so we can't just use
// null values to indicate an invalid pin so we use -1
static const signed long CHANNELS[PIN_COUNT] = {
/* Port A */  -1,           -1,          -1,          -1,          -1,           -1,           -1, -1,
/* Port B */  -1,           -1,          -1,          -1,          ADC_CTL_CH10, ADC_CTL_CH11, -1, -1,
/* Port C */  -1,           -1,          -1,          -1,          -1,           -1,           -1, -1,
/* Port D */  ADC_CTL_CH7,  ADC_CTL_CH6, ADC_CTL_CH5, ADC_CTL_CH4, -1,           -1,           -1, -1,
/* Port E */  ADC_CTL_CH3,  ADC_CTL_CH2, ADC_CTL_CH1, ADC_CTL_CH0, ADC_CTL_CH9,  ADC_CTL_CH8,  -1, -1,
/* Port F */  -1,           -1,          -1,          -1,          -1,           -1,           -1, -1,
};


// Function to initialize an entire module. Checks
// if module has already been intialized. Does not
// actually start any sequences
static void InitializeADCModule(tADCModule *mod) {
    // Check if we have already initialized
    if (mod->initialized)
        return;

    // Reset the queues appropriately
    mod->contQueue = 0;

    mod->singleQueue = 0;
    mod->singleEnd = &mod->singleQueue;
    
    // Enable clocking for ADC
    SysCtlPeripheralEnable(mod->PERIPH);
    
    // Enable hardware oversampling (higher precision results)
    ADCHardwareOversampleConfigure(mod->BASE , ADC_OVERSAMPLING_FACTOR);
    
    // Enable the ADC interrupts, we only use the first two
    IntEnable(mod->INT);
    IntEnable(mod->INT + 1);
    ADCIntEnable(mod->BASE, 0);
    ADCIntEnable(mod->BASE, 1);
    
    mod->initialized = true;
}


// Function to initialize an ADC on a pin, if ADC
// is not supported in hardware on the given pin, 
// then a null pointer is returned
// The returned pointer can be used by the ADCRead functions
tADC *InitializeADC(tPin pin) {
    tADC *adc;

    // If the channel value is -1, that means the pin is not supported
    if (CHANNELS[pin] < 0)
        return 0;
    
    // Grab the next adc and
    // assign it to an adc module
    adc = &adcBuffer[adcCount];
    adc->module = &modBuffer[adcCount % ADC_MODULE_COUNT];
    adcCount++;
    
    // Check for module initialization
    InitializeADCModule(adc->module);
    
    // Setup initial data
    adc->pin = pin;
    
    // Make the pin type ADC
    GPIOPinTypeADC(PORT_VAL(pin), PIN_VAL(pin));
    
    // Return the new adc
    return adc;
}


// Internally used function to setup the continous sequence
static void SetupContinuous(tADCModule *mod, unsigned long trigger) {
    tADC *adc = mod->contQueue;
    int i = 0;
    
    // Create the sequence from scratch
    // decently low priority
    ADCSequenceConfigure(mod->BASE, 0, trigger, 2);
    
    // Create a sequence step for each pin
    for (; adc->next != 0; adc = adc->next)
        ADCSequenceStepConfigure(mod->BASE, 0, i++, CHANNELS[adc->pin]);
    
    // The last pin must also have the interrupt and end flags
    ADCSequenceStepConfigure(mod->BASE, 0, i, ADC_CTL_IE | ADC_CTL_END |
                                              CHANNELS[adc->pin]);
    
    // Just enable the sequence
    ADCSequenceEnable(mod->BASE, 0);
}

// Internally used function to read the next pending single adc.
static void TriggerSingle(tADCModule *mod) {    
    // Create the sequence from scratch
    // priority is just a bit above the continous reads
    ADCSequenceConfigure(mod->BASE, 1, ADC_TRIGGER_PROCESSOR, 1);
    
    // Create the sequence step, which there is only one of
    // with the next adc's pin.
    ADCSequenceStepConfigure(mod->BASE, 1, 0, ADC_CTL_IE | ADC_CTL_END | 
                                              CHANNELS[mod->singleQueue->pin]);
    
    // Enable the sequence and trigger it
    ADCSequenceEnable(mod->BASE, 1);
    ADCProcessorTrigger(mod->BASE, 1);
}


// Interrupt handlers for sequence 0, which is used
// for continous interrupts. Not callbacks need to be called
// and efficiency is the primary concern.
#define ADC_S0_HANDLER(MOD)                         \
void ADC##MOD##SS0Handler(void) {                   \
    tADC *adc = modBuffer[MOD].contQueue;           \
    unsigned long data[8];                          \
    unsigned long *d = data;                        \
                                                    \
    ADCIntClear(ADC##MOD##_BASE, 0);                \
    ADCSequenceDataGet(ADC##MOD##_BASE, 0, data);   \
                                                    \
    for (; adc != 0; adc = adc->next)               \
        adc->value = *d++;                          \
}

ADC_S0_HANDLER(0);
ADC_S0_HANDLER(1);


// Interrupt handlers for sequence 1, which is used
// for single reads. Calls the callback and sets up
// the next pending adc to be read. We also disable the
// sequence here so it can be configured in the next trigger.
#define ADC_S1_HANDLER(MOD)                                 \
void ADC##MOD##SS1Handler(void) {                           \
    tADCModule *mod = &modBuffer[MOD];                      \
    tADC *adc = mod->singleQueue;                           \
                                                            \
    ADCIntClear(ADC##MOD##_BASE, 1);                        \
    ADCSequenceDataGet(ADC##MOD##_BASE, 1, &adc->value);    \
                                                            \
    ADCSequenceDisable(ADC##MOD##_BASE, 1);                 \
                                                            \
    adc->in_callback = true;                                \
    adc->callback(adc->data);                               \
    adc->in_callback = false;                               \
                                                            \
    adc->pending = false;                                   \
    mod->singleQueue = adc->next;                           \
                                                            \
    if (mod->singleQueue)                                   \
        TriggerSingle(mod);                                 \
    else                                                    \
        mod->singleEnd = &mod->singleQueue;                 \
}

ADC_S1_HANDLER(0);
ADC_S1_HANDLER(1);


// Interrupt handler for periodic calls
void ADCTriggerHandler(tADCModule *mod) {
    ADCProcessorTrigger(mod->BASE, 0);
}


// This function sets up an ADC to be run in the background
// A callback can be passed, in which a call to ADCRead 
// will return with the newly obtained value immediately
void ADCBackgroundRead(tADC *adc, tCallback callback, void *data) {
    tADCModule *mod = adc->module;
    
    // Setup the callback data
    adc->callback = callback ? callback : Dummy;
    adc->data = data;
    
    // If we're already reading just return
    if (adc->pending || adc->continous)
        return;
    
    // Flag that we are pending
    adc->pending = true;
    
    // Add us to the queue
    *mod->singleEnd = adc;
    mod->singleEnd = &adc->next;
    adc->next = 0;

    // If nothing was being read, we need to trigger the module
    if (mod->singleQueue == adc)
        TriggerSingle(mod);
}
    

// This function returns the value measured as a percentage
// If the ADC is not continously reading,
// then the function will busy wait for the results
float ADCRead(tADC *adc) {
    // Check if we need to read a value
    if (!adc->in_callback && !adc->continous) {
        // Just call ADCBackgroundRead and busy wait
        ADCBackgroundRead(adc, 0, 0);
        while (adc->pending);
    }
    
    // Calculate the ratio and return it
    return adc->value / (float)(ADC_MAX);
}


// These function set up an ADC to read indefinitly
// Any following calls to ADCRead will return the most recent value
// If the passed time between calls is less than the time it takes for
// the ADC to complete, the ADC will read as fast as possible without overlap
void ADCReadContinuouslyUS(tADC *adc, tTime us) {
    tADCModule *mod = adc->module;
    
    // First check if the module already has continous ADCs
    if (mod->contQueue) {
        // If so, we need to stop it temporarily
        ADCSequenceDisable(mod->BASE, 0);
        
        if (mod->id)
            CallStop(mod->id);
        
        // And set the period to be the smallest value
        if (us < mod->period)
            mod->period = us;

    } else {
        // Otherwise we just set the new period
        mod->period = us;
    }
    
    // We're lazy and order doesn't matter, so we just stick 
    // the new adc in the front of the list
    adc->next = mod->contQueue;
    mod->contQueue = adc;
    
    // Check if the given speed is too fast
    if (mod->period <= ADC_TIME * ADC_OVERSAMPLING_FACTOR) {
        // In which case we set it to always be triggered 
        // and clear the id
        SetupContinuous(mod, ADC_TRIGGER_ALWAYS);
        mod->id = 0;
        
    } else {
        // Just setup the sequence to trigger on a
        // scheduled interrupt for the given time
        SetupContinuous(mod, ADC_TRIGGER_PROCESSOR);
        mod->id = CallEveryUS(ADCTriggerHandler, mod, mod->period);
    }

    // Don't forget to set the continous flag
    adc->continous = true;
}
    
void ADCReadContinuously(tADC *adc, float s) {
    ADCReadContinuouslyUS(adc, US(s));
}
