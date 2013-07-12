
#include "init.h"
#include "time.h"
#include "adc.h"
#include "uart.h"
#include "servo.h"

AddServo(One,A,2)
int ServoMain(void){
    InitializeMCU();
    InitializeUART();
    InitializeServoOne();
    UARTprintf("Servo Test Starting...\n");
    for(;;){
        static int i = 0;
        SetServoOne(i);
        UARTprintf("Servo Output: %d   \r",i);
        i = (i+1)%256;
        WaitMS(100);
    }
}

int ADCMain(void){
    InitializeMCU();
    InitializeUART();
    InitializeADC();
    UARTprintf("ADC Test Starting...\n");
    for(;;){
        UARTprintf("ADC0: %d ADC1: %d     \r", GetADC(0), GetADC(1));
    }
}

/*** TIME TESTS ***/
int BusyWaitMain(void){
    InitializeMCU();
    InitializeUART();
    UARTprintf("Busy Wait Test Starting...\n");
    for(;;){
        static int count = 0;
        UARTprintf("1/10 Seconds past: %d     \r", count++);
        Wait(.1);
    }
}
int SysTimeMain(void){
    InitializeMCU();
    InitializeUART();
    UARTprintf("System Time Test Starting...\n");
    for(;;){
        UARTprintf("S: %d mS: %d      \r", g_ulSystemTimeSeconds, g_ulSystemTimeMS);
    }
}

void perFuncTest(void){
    static int count = 0;
    UARTprintf("Count: %d      \r", count++);
}

int PerFuncMain(void){
    InitializeMCU();
    InitializeUART();
    UARTprintf("Periodic Functions Test Starting...\n");
    AddPeriodicFunction(perFuncTest,1000);
    for(;;);
}


int main(void){
    return PerFuncMain();
}
