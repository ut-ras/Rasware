#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/uart.h>

void uartDemo(void) {
    char charArray[100];
    char ESC = 27;
    char ch = 0;
    while(ch != ESC) {
        Printf("tell me something! (science)\n-> ");
        Gets(charArray, 100);
        Printf("you said, \"%s\"\n", charArray);
        Printf("Thanks for the input! Press ESC to exit, or any other key to continue\n");
        ch = Getc();
    }
}
