
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
        static float i = 0;
        SetServoOne(i);
        SetServoTwo(i);
        UARTprintf("Servo Output: %d   \r",i);
        i += (1)/((float)SERVO_GENERATOR_RESOLUTION);
        if(i>1) i=0;
        WaitMS(1);
    }
}

int ADCMain(void){
    InitializeMCU();
    InitializeUART();
    UARTprintf("\nADC Test Starting...\n");
    for(;;){
        //UARTprintf("ADC0(PE3): %d \t ADC1(PE2): %d \t ADC2(PE1): %d \t ADC3(PE0): %d \t ADC4(PD4): %d \t ADC5(PD3): %d \t ADC6(PD1): %d \t ADC7(PD0): %d \t ADC8(PE5): %d \t ADC9(PE4): %d \t ADC10(PB4): %d \t ADC11(PB5): %d             \r", 
        UARTprintf("%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d           \r", 
        GetADC(ADC0), GetADC(ADC1), 
        GetADC(ADC2), GetADC(ADC3), 
        GetADC(ADC4), GetADC(ADC5),        
        GetADC(ADC6), GetADC(ADC7),
        GetADC(ADC8), GetADC(ADC9),
        GetADC(ADC10), GetADC(ADC11));
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
