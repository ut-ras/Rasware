#include <inc/hw_types.h>		// tBoolean
#include <utils/uartstdio.h>	// input/output over UART

//#include <stdio.h>

#include <RASLib/inc/uart.h> 
#include <RASLib/inc/init.h> 
#include <RASLib/inc/time.h>
#include <RASLib/inc/encoder.h>
#include <RASLib/inc/json_protocol.h>

char buffer[20];
    
char* encoderPublisher(void* data) {
    //sprintf(buffer, "%ld", GetEncoder((tEncoder*)data));
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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
    
    UARTprintf("um hi?\n");

    //test();
    while(1);
}
