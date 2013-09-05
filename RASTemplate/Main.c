#include <inc/hw_types.h>
#include <RASLib/inc/uart.h>
#include <RASLib/inc/linesensor.h> 
#include <RASLib/inc/init.h> 
#include <RASLib/inc/time.h>

// The 'main' function is the entry point of the program
int main(void) {
    
    InitializeMCU();
    InitializeUART();
    
    // Initialization code can go here
    
    while (1) {
        // Runtime code can go here
    }
}
