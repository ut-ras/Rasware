
#include "init.h"
#include "math.h"
#include <RASLib/inc/uart.h>

int main(void){

	//unsigned long ulTest = 1289719;
	float fTest = 1234567.1234567;
	double dTest = 1234567.1234567;
	
	InitializeMCU();
	InitializeUART();
	
	Printf("\nLoaded, starting tests:\n");
	
	// UART test
	//Printf(" %% ", (int)ulTest);
	//Printf(" %c ", 'c');
	//Printf(" %s ", "string");
	//Printf(" %d ", sizeof(double));
	//Printf(" %i\n", ulTest);
	Printf(" %F ", dTest);
	Printf(" %A \n", dTest);
	Printf(" %3F ", dTest);
	Printf(" %8F ", dTest);
	Printf(" %08F \n", dTest);
	Printf(" %3.1F ", dTest);
	Printf(" %8.2F ", dTest);
	Printf(" %08.3F \n", dTest);
	Printf(" %F ", fTest);
	Printf(" %A \n", fTest);
	Printf(" %3F ", fTest);
	Printf(" %8F ", fTest);
	Printf(" %08F \n", fTest);
	Printf(" %3.1F ", fTest);
	Printf(" %8.2F ", fTest);
	Printf(" %08.3F \n", fTest);
	Printf("%f ", 1.0/0.0);
	Printf("%f ", 0.0/0.0);
	Printf("%f\n", -1.0/0.0);
	Printf("%E\n", 1e30f);
	Printf("%.2e ", .00000123);
	Printf("%f\n", .00000123);
	
	while(1)
	{
	}
}
