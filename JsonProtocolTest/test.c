#include <stdlib.h>                     // atoi
#include <inc/hw_types.h>		        // tBoolean
#include <RASLib/inc/init.h>            // InitializeMCU
#include <RASLib/inc/uart.h>            // InitializeUART, Printf
#include <RASLib/inc/rasstring.h>       // SPrintf
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/json_protocol.h>

char msgBuffer[20];

tPub myPub;
tSub mySub;

int counter;

char* pubHandler(void* data) {
    return msgBuffer;
}

void subHandler(void* data, char *jsonvalue) {
    SPrintf(msgBuffer, "%s", jsonvalue);
}

int main(void) {
    InitializeMCU();
    InitializeUART();
    
    InitializePublisher(&myPub, "counter", 0, pubHandler);
    InitializeSubscriber(&mySub, "counter", 0, subHandler);
    BeginPublishing(.1);
    BeginSubscribing(.1);
    
    while(1);
}
