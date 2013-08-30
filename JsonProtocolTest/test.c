#include <inc/hw_types.h>		// tBoolean
#include <RASLib/inc/uart.h>
#include <RASLib/inc/init.h> 
#include <RASLib/inc/time.h>
#include <RASLib/inc/encoder.h> 
#include <RASLib/inc/json_protocol.h> 
#include <RASLib/inc/rasstring.h>

char buffer[20];
    
char* encoderPublisher(void* data) {
    int ticks = GetEncoder((tEncoder*)data);
    
    SPrintf(buffer, "%d", ticks);
    
    return buffer;
}
    
void test(void) {
    tEncoder *rightEnc = InitializeEncoder(PIN_B0, PIN_B1),
             *leftEnc = InitializeEncoder(PIN_B2, PIN_B3);

    AddPublisher("right_encoder", rightEnc, encoderPublisher);
    AddPublisher("left_encoder", leftEnc, encoderPublisher);

    BeginPublishing(.1);
}

int main(void) {
    InitializeMCU();
	InitializeUART();
    
    test();
    while(1);
}
