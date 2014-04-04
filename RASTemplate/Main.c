#include <RASLib/inc/common.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/servo.h>

tServo *m0;
tServo *m1;
tServo *m2;
tServo *m3;

tServo *c0;
tServo *c1;
tServo *c2;

unsigned char cmd;
unsigned char val;

#define MSCALE(m) (m/255.0)


void blink(void) {
    static tBoolean led = false;
    SetPin(PIN_F2, led);
    led = !led;
}

void kill(void) {
    SetServo(m0, 0.5);
    SetServo(m1, 0.5);
    SetServo(m2, 0.5);
    SetServo(m3, 0.5);

    SetServo(c0, 0.5);
    SetServo(c1, 0.5);
    SetServo(c2, 0.5);
}


// The 'main' function is the entry point of the program
int main(void) {
    InitializeMCU();

    Printf("Geky's hack enabled\n");
    CallEvery(blink, 0, 0.25);

    m0 = InitializeServo(PIN_D0);
    m1 = InitializeServo(PIN_D1);
    m2 = InitializeServo(PIN_D2);
    m3 = InitializeServo(PIN_D3);

  //  c0 = InitializeServo(PIN_C4);
 //   c1 = InitializeServo(PIN_C5);
//    c2 = InitializeServo(PIN_C6);

    while (1) {
        cmd = Getc();
        val = Getc();

        if (Getc() != '\n') {
            kill();
        }

        switch (cmd) {
            case 'x':
            kill();
            break;

            case 'a':
            SetServo(m0, MSCALE(val));
            break;

            case 'b':
            SetServo(m1, MSCALE(val));
            break;

            case 'c':
            SetServo(m2, MSCALE(val));
            break;

            case 'd':
            SetServo(m3, MSCALE(val));
            break;
        }
    }
}
