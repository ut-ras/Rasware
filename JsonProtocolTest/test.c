#include <stdlib.h>                     // atoi
#include <inc/hw_types.h>		        // tBoolean
#include <RASLib/inc/init.h>            // InitializeMCU
#include <RASLib/inc/uart.h>            // InitializeUART, Printf
#include <RASLib/inc/rasstring.h>       // SPrintf
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/json_protocol.h>

char msgBuffer[20];
tPub rightEncPub, leftEncPub;
tSub rightMotorSub, leftMotorSub;

char* encoderPubHandler(void* data) {
    SPrintf(msgBuffer, "%d", GetEncoder((tEncoder*)data));
    
    return msgBuffer;
}

void setupPublishing(void) {
    tEncoder *rightEnc = InitializeEncoder(PIN_B0, PIN_B1),
             *leftEnc = InitializeEncoder(PIN_B2, PIN_B3);

    InitializePublisher(&rightEncPub, "right_encoder", rightEnc, encoderPubHandler);
    InitializePublisher(&leftEncPub, "left_encoder", leftEnc, encoderPubHandler);

    BeginPublishing(.1);
}

void motorSubHandler(void* data, char* jsonvalue) {
    int power = atoi(jsonvalue);

    // expecting a power level in the range [-100,100]
    if (power < -100 || power > 100) {
        Printf("received unexpected motor power level: %d\n", power);
        return; 
    }

    SetMotor((tMotor*)data, power/100.0);
}

void setupSubscribing(void) {
    tMotor *rightMotor = InitializeMotor(PIN_B6, PIN_B7, true),
           *leftMotor = InitializeMotor(PIN_E5, PIN_E4, true);

    InitializeSubscriber(&rightMotorSub, "right_motor", rightMotor, motorSubHandler);
    InitializeSubscriber(&leftMotorSub, "left_motor", leftMotor, motorSubHandler);

    BeginSubscribing(.1);
}

int main(void) {
    InitializeMCU();
    InitializeUART();
    
    setupPublishing();
    setupSubscribing();
    
    while(1);
}
