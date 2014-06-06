#include <stdlib.h>                     // atoi
#include <inc/hw_types.h>		        // tBoolean
#include <RASLib/inc/common.h>            // InitializeMCU
#include <RASLib/inc/uart.h>            // InitializeUART, Printf
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>

#include <StellarisWare/inc/hw_memmap.h>
#include <StellarisWare/inc/hw_sysctl.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/inc/hw_watchdog.h>
#include <StellarisWare/driverlib/watchdog.h>

tMotor *left_motor, *right_motor;
tEncoder *left_encoder, *right_encoder;
tBoolean LED=true;

void blinkLED(void) {
    SetPin(PIN_F2, LED);
    LED = !LED;
}

void WatchdogHandler(void) {
    WatchdogIntClear(WATCHDOG_BASE);
    WatchdogResetDisable(WATCHDOG_BASE);

    SetMotor(left_motor, 0);
    SetMotor(right_motor, 0);
    SetPin(PIN_F1, true);
}

void WatchDog_Init(void) {
    SetPin(PIN_F1, false);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG);  // Watchdog timer
    WatchdogIntClear(WATCHDOG_BASE);
    //WatchdogStallEnable(WATCHDOG_BASE);   // Enable for breakpoint debugging
    if(WatchdogLockState(WATCHDOG_BASE) == true) {
        WatchdogUnlock(WATCHDOG_BASE);
    }

    WatchdogIntRegister(WATCHDOG_BASE, WatchdogHandler);

    //Motor control node communicates at 10Hz, if no command for 20Hz halts
    WatchdogReloadSet(WATCHDOG_BASE, 2500000);   

    WatchdogIntEnable(WATCHDOG_BASE);
    WatchdogResetDisable(WATCHDOG_BASE);   //Second interrupt doesn't result processor

    WatchdogEnable(WATCHDOG_BASE);
}

static int ReadLine(char *line, int max_len) {
    int i = 0;
    char ch = 0;
    
    while (i < max_len && ch != '\n') {
        ch = Getc();
        line[i] = ch;
        i += 1;
    }
    
    line[max_len] = 0;
    return i;
}

int CSIZE = 7;

int checkFormat(char* buf, int numbytes) {
    int i;
    int knownis[3] = {0, 1+CSIZE, 1+CSIZE+1+CSIZE};
    char knowncs[3] = {'#', ',', '#'};
    
    if (numbytes != 1 + CSIZE + 1 + CSIZE + 1 + 1) {
        return 0;
    }

    for (i = 0; i < 3; i++) {
        if (buf[knownis[i]] != knowncs[i]) {
            return false;
        } 
    }
    
    return 1;
}

struct PIDStruct {
    tMotor* motor;
    tEncoder* enc;
    float prevCommand;
    signed long prevTicks;
    float prevErr; 
    float accumErr;
    signed long deltaTicks;
} typedef PIDStruct;

#define PIDP (0.00003)
#define PIDD (0.00013)
#define PIDI (0.000000)
#define MAX_MOTOR (.5)
#define MIN_MOTOR (-.5)

void runPID(PIDStruct* s, int goalDeltaTicks, tMotor* motor) {
    signed long ticks = -GetEncoder(s->enc);
    signed long deltaTicks = ticks - s->prevTicks;
   
    float err = goalDeltaTicks - deltaTicks;
    s->accumErr += err;
 
    float pidOutput = err*PIDP + (err - s->prevErr)*PIDD + s->accumErr*PIDI;
    float motorCommand = s->prevCommand + pidOutput;
    
    if (motorCommand > MAX_MOTOR) motorCommand = MAX_MOTOR;
    if (motorCommand < MIN_MOTOR) motorCommand = MIN_MOTOR;

    SetMotor(motor, motorCommand);
   
    s->prevTicks = ticks;
    s->prevErr = err;
    s->prevCommand = motorCommand;
    s->deltaTicks = deltaTicks;
}

int main(void) {
    int i;
    InitializeMCU();
     
    left_motor = InitializeServoMotor(PIN_A6, false);
    right_motor = InitializeServoMotor(PIN_A7, false);

    right_encoder = InitializeEncoder(PIN_E4, PIN_E5, false);   
    left_encoder = InitializeEncoder(PIN_B0, PIN_B1, false);

    ResetEncoder(left_encoder);
    ResetEncoder(right_encoder);

    CallEvery(blinkLED,0,0.25f);
    
    WatchDog_Init();
    
    {
    PIDStruct right = {right_motor, right_encoder, 0.0, 0L, 0.0, 0.0, 0L};
    PIDStruct left = {left_motor, left_encoder, 0.0, 0L, 0.0, 0.0, 0L};

    while (1) {
        char buf[100] = {0};
        int numbytes = ReadLine(buf, 100);
        
        if (!checkFormat(buf, numbytes)) {
            Puts("bad format!\n");
        } else {
            int goalDeltaTicksRight, goalDeltaTicksLeft;
 
            buf[1 + CSIZE] = 0;
            buf[1 + CSIZE + 1 + CSIZE] = 0;
            goalDeltaTicksRight = atoi(&buf[1]);
            goalDeltaTicksLeft = atoi(&buf[1 + CSIZE + 1]);
            
            runPID(&right, goalDeltaTicksRight, right_motor);
            runPID(&left, goalDeltaTicksLeft, left_motor);
            
            Printf(
                "{\"received\":{\"right\":%d,\"left\":%d},"
                "\"motors\":{\"right\":%f,\"left\":%f},"
                "\"deltas\":{\"right\":%d,\"left\":%d}}\n",
                goalDeltaTicksRight, goalDeltaTicksLeft, 
                right.prevCommand, left.prevCommand, 
                right.deltaTicks, left.deltaTicks);
            
            WatchdogReloadSet(WATCHDOG_BASE, 25000000);    
        }
    }
    }
}
