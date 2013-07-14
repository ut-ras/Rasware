
#include "init.h"
#include "time.h"
#include "adc.h"
#include "uart.h"
#include "servo.h"
#include "encoder.h"
#include "motor.h"
AddServo(One,B,6)
AddServo(Two,B,7)

int EncoderMain(void){
    InitializeMCU();
    InitializeUART();
    InitializeEncoder(F,2,3);
    UARTprintf("Encoder Test Starting...\n");
    for(;;){
        UARTprintf("enc0: %d     \r", GetEncoderTicks(0));
    }
}

// Rachel's board's pinout
AddMotor(One,A,4,C,6,COAST)
AddMotor(Two,A,3,A,2,COAST)
AddMotor(Three,E,5,E,4,COAST)
AddMotor(Four,C,7,A,5,COAST)
int MotorMain(void){
    InitializeMCU();
    InitializeUART();
    InitializeMotorOne();
    InitializeMotorTwo();
    InitializeMotorThree();
    InitializeMotorFour();
    UARTprintf("\nMotor Test Starting...\n");
    for(;;){
        static float i = -1.0;
        UARTprintf("Motor Output: %g %d   \r",i,(long)(i*10000));
        SetMotorOne(i);
        SetMotorTwo(i);
        SetMotorThree(i);
        SetMotorFour(i);
        i += (1)/((float)MOTOR_GENERATOR_RESOLUTION);
        if(i>1) i=-1;
        WaitMS(1);
    }
}
int ServoMain(void){
    InitializeMCU();
    InitializeUART();
    InitializeServoOne();
    InitializeServoTwo();
    UARTprintf("Servo Test Starting...\n");
    for(;;){
        static int i = 0;
        SetServoOne(i);
        SetServoTwo((i*2)%256);
        UARTprintf("Servo Output: %d   \r",i);
        i = (i+1)%256;
        WaitMS(100);
    }
}

int ADCMain(void){
    InitializeMCU();
    InitializeUART();
    InitializeADC();
    UARTprintf("\nADC Test Starting...\n");
    for(;;){
        UARTprintf("ADC0: %d ADC1: %d     \r", GetADC(0), GetADC(1));
    }
}

/*** TIME TESTS ***/
int BusyWaitMain(void){
    InitializeMCU();
    InitializeUART();
    UARTprintf("\nBusy Wait Test Starting...\n");
    for(;;){
        static int count = 0;
        UARTprintf("1/10 Seconds past: %d     \r", count++);
        Wait(.1);
    }
}
int SysTimeMain(void){
    InitializeMCU();
    InitializeUART();
    UARTprintf("\nSystem Time Test Starting...\n");
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
    UARTprintf("\nPeriodic Functions Test Starting...\n");
    AddPeriodicFunction(perFuncTest,1000);
    for(;;);
}


int main(void){
    return ADCMain();
}
