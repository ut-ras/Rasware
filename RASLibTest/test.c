
#include "init.h"
#include <RASLib/inc/uart.h>

int main(void){

	//unsigned long ulTest = 1289719;
	float fTest = 1234567.1234567;
	double dTest = 1234567.1234567;
	
	InitializeMCU();
	InitializeUART();
	
	printf("\nLoaded, starting tests:\n");
	
	// UART test
	//printf(" %% ", (int)ulTest);
	//printf(" %c ", 'c');
	//printf(" %s ", "string");
	//printf(" %d ", sizeof(double));
	//printf(" %i\n", ulTest);
	printf(" %F ", dTest);
	printf(" %A \n", dTest);
	printf(" %3F ", dTest);
	printf(" %8F ", dTest);
	printf(" %08F \n", dTest);
	printf(" %3.1F ", dTest);
	printf(" %8.2F ", dTest);
	printf(" %08.3F \n", dTest);
	printf(" %F ", fTest);
	printf(" %A \n", fTest);
	printf(" %3F ", fTest);
	printf(" %8F ", fTest);
	printf(" %08F \n", fTest);
	printf(" %3.1F ", fTest);
	printf(" %8.2F ", fTest);
	printf(" %08.3F \n", fTest);
	printf("%f ", 1.0/0.0);
	printf("%f ", 0.0/0.0);
	printf("%f\n", -1.0/0.0);
	printf("%f", 1e30f);
	
	while(1)
	{
	}
}
