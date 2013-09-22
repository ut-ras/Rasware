#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/uart.h>

void uartDemo(void) {
  Printf("Tell me something!\n-> ");
  
  {
    char charArray[100];
    Gets(charArray, 100);
    Printf("You said, \"%s\"\n", charArray);
    Printf("Thanks for the input!\n");
  }
  
  {
    char newline = 13;
    char ch = Getc();
    while(ch != newline) {
      ch = Getc();
      Putc(ch);
    }
  }
}
