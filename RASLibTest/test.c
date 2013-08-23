
#include "init.h"
#include <RASLib/inc/uart.h>

int main(void){

	unsigned long ulTest = 1289719;
	double dTest = -1.125;
	
	InitializeMCU();
	InitializeUART();
	
	printf("\nLoaded, starting tests:\n");
	
	// UART test
	printf(" %% ", (int)ulTest);
	printf(" %c ", 'c');
	printf(" %s ", "string");
	printf(" %d ", sizeof(double));
	printf(" %i\n", ulTest);
	printf(" %F ", dTest);
	printf(" %A ", dTest);
	printf(" %E ", dTest);
	
	while(1)
	{
	}
}
