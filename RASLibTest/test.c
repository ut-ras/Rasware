
#include "init.h"
#include <RASLib/inc/uart.h>

int main(void){

	unsigned long ulTest = 1289719;
	
	InitializeMCU();
	InitializeUART();
	
	printf("\nLoaded, starting tests:\n");
	
	// UART tests
	printf(" %% ", (int)ulTest);
	printf(" %c ", 'c');
	printf(" %s ", "string");
	printf(" %d ", (int)ulTest);
	printf(" %i ", ulTest);
	printf(" %x ", ulTest);
	printf(" %X ", ulTest);
	
	while(1)
	{
	}
}
