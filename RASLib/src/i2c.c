//*******************************
// I2C Functions and Defines 
//*******************************

#include <stdlib.h>
#include <stdarg.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/i2c.h>
#include "i2c.h"
#include "gpio.h"
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <inc/hw_i2c.h>

// Definition of struct I2C
// Defined to tI2C in i2c.h
struct I2C {
    // Constant StellarisWare values 
    // specific to each module
    const unsigned long BASE;
    const unsigned long PERIPH;
    
    // Sending and recieving data
    unsigned char *data;
    unsigned int len;
    
    // Callback data
    tCallback callback;
    void *cbdata;
    
    // State machine
    volatile enum {
        SENDING,
        RECEIVING,
        DONE
    } state;
};

// Buffer of I2C structs to use
// Limited to the available modules
tI2C i2cBuffer[] = {
    {I2C0_MASTER_BASE, SYSCTL_PERIPH_I2C0},
    {I2C1_MASTER_BASE, SYSCTL_PERIPH_I2C1},
    {I2C2_MASTER_BASE, SYSCTL_PERIPH_I2C2},
    {I2C3_MASTER_BASE, SYSCTL_PERIPH_I2C3},
    {I2C4_MASTER_BASE, SYSCTL_PERIPH_I2C4},
    {I2C5_MASTER_BASE, SYSCTL_PERIPH_I2C5},
};

int i2cCount = 0;


// Function to initialize an I2C module on a pair of pins
// The returned pointer can be used by the Send and Recieve functions
tI2C *InitializeI2C(tPin sda, tPin scl) {
    // Grab the next module
    tI2C *i2c = &i2cBuffer[i2cCount++];
    
    // Enable the peripheral
    SysCtlPeripheralEnable(i2c->PERIPH);
    
    // Setup the pins as specified
    GPIOPinTypeI2C(PORT_VAL(sda), PIN_VAL(sda));
    GPIOPinTypeI2CSCL(PORT_VAL(scl), PIN_VAL(scl));
    
    // Setup the clock
    I2CMasterInitExpClk(i2c->BASE, SysCtlClockGet(), false);
    
    // Enable the I2C module
    I2CMasterEnable(i2c->BASE);
    
    // Return the initialized module
    return i2c;
}


// Interrupt handlers for i2c
// Depending on the state of the state machine,
// executes the next action.
#define I2C_HANDLER(MOD)                                                \
void I2C##MOD##Handler(void) {                                          \
    tI2C *i2c = &i2cBuffer[MOD];                                        \
                                                                        \
    I2CMasterIntClear(I2C##MOD##_MASTER_BASE);                          \
                                                                        \
    switch(i2c->state) {                                                \
        case SENDING:                                                   \
            if (i2c->len == 0) {                                        \
                i2c->state = DONE;                                      \
                i2c->callback(i2c->cbdata);                             \
                return;                                                 \
            }                                                           \
                                                                        \
            I2CMasterDataPut(I2C##MOD##_MASTER_BASE, *i2c->data);       \
            i2c->data++;                                                \
            i2c->len--;                                                 \
                                                                        \
            if (i2c->len > 0)                                           \
                I2CMasterControl(I2C##MOD##_MASTER_BASE,                \
                                 I2C_MASTER_CMD_BURST_SEND_CONT);       \
            else                                                        \
                I2CMasterControl(I2C##MOD##_MASTER_BASE,                \
                                 I2C_MASTER_CMD_BURST_SEND_FINISH);     \
                                                                        \
            break;                                                      \
                                                                        \
        case RECEIVING:                                                 \
            *i2c->data = I2CMasterDataGet(I2C##MOD##_MASTER_BASE);      \
            i2c->data++;                                                \
            i2c->len--;                                                 \
                                                                        \
            if (i2c->len == 0) {                                        \
                i2c->state = DONE;                                      \
                i2c->callback(i2c->cbdata);                             \
                return;                                                 \
            }                                                           \
                                                                        \
            if (i2c->len > 1)                                           \
                I2CMasterControl(I2C##MOD##_MASTER_BASE,                \
                                 I2C_MASTER_CMD_BURST_RECEIVE_CONT);    \
            else                                                        \
                I2CMasterControl(I2C##MOD##_MASTER_BASE,                \
                                 I2C_MASTER_CMD_BURST_RECEIVE_FINISH);  \
                                                                        \
            break;                                                      \
    }                                                                   \
}

I2C_HANDLER(0);
I2C_HANDLER(1);
I2C_HANDLER(2);
I2C_HANDLER(3);
I2C_HANDLER(4);
I2C_HANDLER(5);


// This function sends data to an I2C address.
// A callback can be passed and will be called when 
// all of the data in the passed array is sent.
void I2CBackgroundSend(tI2C *i2c, unsigned char addr, 
                                  unsigned char *data, unsigned int len,
                                  tCallback callback, void *cbdata) {
	// Make sure data is actually being sent
    if (len < 1)
        return;
    
    // We loop here while the bus is busy
    // correct sending behaviour should be implemented
    // at a higher level
    while (i2c->state != DONE);
    i2c->state = SENDING;
    
    // Assign the address
    I2CMasterSlaveAddrSet(i2c->BASE, addr, false);
    
    // Initialize callback information
    i2c->callback = callback ? callback : Dummy;
    i2c->cbdata = cbdata;
    
    // Start sending data
    I2CMasterDataPut(i2c->BASE, *data);
    i2c->data = data + 1;
    i2c->len = len - 1;
    
    // Either send a single command or start sending multiple
    if (len == 1)
        I2CMasterControl(i2c->BASE, I2C_MASTER_CMD_SINGLE_SEND);
    else
        I2CMasterControl(i2c->BASE, I2C_MASTER_CMD_BURST_SEND_START);
    
    // The finite state machine implemented in the interrupt
    // handler will take over from here
}


// Summary:	Sends 'len' number of characters to specified address
// Parameters:
//		addr:	address to send data to
//      data:   pointer to memory location to read data
//		len:	number of characters being sent
// Note:	Number of characters must be equal to 'len'
void I2CSend(tI2C *i2c, unsigned char addr, 
                        unsigned char *data, unsigned int len) {
    I2CBackgroundSend(i2c, addr, data, len, 0, 0);
    while (i2c->state != DONE);
}


// This function receives data from an I2C address.
// A callback can be passed and will be called when 
// all of the data is loaded into the passed array.
void I2CBackgroundReceive(tI2C *i2c, unsigned char addr, 
                                     unsigned char *data, unsigned int len,
                                     tCallback callback, void *cbdata) {
	// Make sure data is actually being retrieve
    if (len < 1)
        return;
    
    // We loop here while the bus is busy
    // correct recieving behaviour should be implemented
    // at a higher level
    while (i2c->state != DONE);
    i2c->state = RECEIVING;
    
    // Assign the address
    I2CMasterSlaveAddrSet(i2c->BASE, addr, true);
    
    // Initialize callback information
    i2c->callback = callback ? callback : Dummy;
    i2c->cbdata = cbdata;
    
    // Either read single byte of data or multiple
    if (len == 1)
        I2CMasterControl(i2c->BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    else
        I2CMasterControl(i2c->BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    
    // The finite state machine implemented in the interrupt
    // handler will take over from here
}


// Summary:	Recieve/Fetch data from specified address
// Parameters:
//		addr:	address to recieve data from
//		data:	pointer to memory location to save data
//		len:	number of cahracers that will be recieved
extern void I2CRecieve(tI2C *i2c, unsigned char addr, 
                                  unsigned char* data, unsigned int len) {
    I2CBackgroundReceive(i2c, addr, data, len, 0, 0);
    while (i2c->state != DONE);
}
