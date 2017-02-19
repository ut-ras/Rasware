//*****************************************************************************
//
// timeout - functions for using timeouts
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

#include "timeout.h"

#include <stdint.h>
#include <stddef.h>

#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_watchdog.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/watchdog.h>
#include <StellarisWare/driverlib/interrupt.h>

struct timeout {
        uint32_t baseR;
        tTime timeout;
        tCallback toCall;
        void * data;
};

static struct timeout timeouts[TIMEOUT_COUNT] = { {WATCHDOG_BASE, 0, NULL, NULL}, };
static uint32_t numTimeoutsAllocated = 0;

void WatchdogHandler(void);

void InitializeSystemTimeout(void) {
        /* in the event that there is more than one watchdog timer, we should really
         * conseder rewriting this
         */
        SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG);
        WatchdogIntClear(WATCHDOG_BASE);
        WatchdogStallEnable(WATCHDOG_BASE);
        WatchdogIntRegister(WATCHDOG_BASE, WatchdogHandler);
}

int CallOnTimeout(tCallback callback, void * data, float s){
        return CallOnTimeoutUS(callback, data, US(s));
}

int CallOnTimeoutUS(tCallback callback, void *data, tTime us) {
        int timeoutNum = numTimeoutsAllocated;
        if (timeoutNum >= TIMEOUT_COUNT) return 0;
        ++numTimeoutsAllocated;
        timeouts[timeoutNum].data = data;
        timeouts[timeoutNum].toCall = callback;
        timeouts[timeoutNum].timeout = us * 100;
        if(WatchdogLockState(timeouts[timeoutNum].baseR) == true) {
                WatchdogUnlock(timeouts[timeoutNum].baseR);
        }
        TimeoutReset(timeoutNum + 1);

        WatchdogIntEnable(timeouts[timeoutNum].baseR);
        WatchdogResetDisable(timeouts[timeoutNum].baseR); //Second interrupt doesn't result processor

        WatchdogEnable(timeouts[timeoutNum].baseR);
        return timeoutNum + 1;
}

void TimeoutReset(int timeoutid){
        if (timeoutid > 0 && timeoutid - 1 < numTimeoutsAllocated)
                WatchdogReloadSet(timeouts[timeoutid - 1].baseR,
                                  timeouts[timeoutid - 1].timeout);
}

void TimeoutStop(int timeoutid){
        if (timeoutid > 0 && timeoutid - 1 < numTimeoutsAllocated) {
                /* surprisingly, there seams to me no disable function in stellarisware...
                WatchdogDisable(timeouts[timeoutid - 1].baseR);
                */
                --numTimeoutsAllocated;
        }
}

void WatchdogHandler(void){
        WatchdogIntClear(WATCHDOG_BASE);
        WatchdogResetDisable(WATCHDOG_BASE);
        timeouts[0].toCall(timeouts[0].data);
}
