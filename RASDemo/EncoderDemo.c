#include "RASDemo.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/encoder.h>


tEncoder *demoEncoder[2];

void initEncoders(void) {
    demoEncoder[0] = InitializeEncoder(PIN_D0, PIN_D1, false);
    demoEncoder[1] = InitializeEncoder(PIN_D2, PIN_D3, false);
}

void encoderDemo(void) {
    Printf("Press:\nany key-read encoder values\n");
    Printf("any key after read begins-quit\n");
    
    ResetEncoder(demoEncoder[0]);
    ResetEncoder(demoEncoder[1]);

    while(!KeyWasPressed()) {
    Printf("enc0:%d  enc1:%d\r",
        GetEncoder(demoEncoder[0]),
        GetEncoder(demoEncoder[1]));
      }
    
      Printf("\n");
}
