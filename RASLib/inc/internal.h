// Note: this code is for internal use only. Projects that use RASLib should never include it.

#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include "inc/hw_types.h"

// Definition of callback function pointer
typedef void (*tCallback)(void*);

// Does nothing; used as placeholder in time.c and gpioints.c
void Dummy(void*);

#endif // __INTERNAL_H__
