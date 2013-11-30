#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>

tEncoder *leftEncoder;
tEncoder *rightEncoder;

void initEncoders(void) {
    leftEncoder = InitializeEncoder(PIN_D0, PIN_D1, false);
    rightEncoder = InitializeEncoder(PIN_D2, PIN_D3, false);
}

void encoderDemo(void) {
    Printf("Press any key to quit\n");
    
    ResetEncoder(leftEncoder);
    ResetEncoder(rightEncoder);

    while (!KeyWasPressed()) {
        Printf(
            "Encoder values:  %10d  %10d\r",
            GetEncoder(leftEncoder),
            GetEncoder(rightEncoder)
            );
    }
    
    Printf("\n");
}

