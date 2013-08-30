#include <inc/hw_types.h>		// tBoolean
#include <RASLib/inc/uart.h>
#include <RASLib/inc/init.h> 
#include <RASLib/inc/time.h>
#include <RASLib/inc/encoder.h> 
#include <RASLib/inc/json_protocol.h> 
#include <RASLib/inc/rasstring.h>

char msgBuffer[20];

tPub rightEncPub, leftEncPub;
    
char* encoderPublisher(void* data) {
    SPrintf(msgBuffer, "%d", GetEncoder((tEncoder*)data));
    
    return msgBuffer;
}

void setupPublishing(void) {
    tEncoder *rightEnc = InitializeEncoder(PIN_B0, PIN_B1),
             *leftEnc = InitializeEncoder(PIN_B2, PIN_B3);

    InitializePublisher(&rightEncPub, "right_encoder", rightEnc, encoderPublisher);
    InitializePublisher(&leftEncPub, "left_encoder", leftEnc, encoderPublisher);

    BeginPublishing(.1);
}

int main(void) {
    InitializeMCU();
    InitializeUART();
    
    setupPublishing();
    
    while(1);
}
