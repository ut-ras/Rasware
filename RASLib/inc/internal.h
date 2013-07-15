#include "gpioints.h"

// Does nothing; used as placeholder in time.c and gpioints.c
void Dummy(void);

// Note: it is not recommended to call this function directly. Instead, use the
// AddServo macro above to generate a unique InitializeServo function
unsigned long AddServoFunction(unsigned long port, unsigned long pin);

// Note: it is not recommended to call this function directly. Instead, use the
// AddServo macro above to generate a unique SetServo function
void SetServoPosition(unsigned long index, float input);

// Note: it is not recommended to call this function directly. Instead, use the
// InitializeEncoder macro above
int AddEncoder(unsigned long ulPeripheral,
               unsigned long ulPort,
               unsigned char ucPinA,
               unsigned char ucPinB,
               unsigned long ulInterrupt,
               tGPIO_Port port);
