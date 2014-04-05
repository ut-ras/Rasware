#include <RASLib/inc/common.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/adc.h>
#include <StellarisWare/driverlib/gpio.h>

tServo *m0;
tServo *m1;
tServo *m2;
tServo *m3;
#define p0 PIN_D0
#define p1 PIN_D1
#define p2 PIN_D2
#define p3 PIN_D3

tServo *c0;
tServo *c1;
tServo *c2;
#define q0 PIN_C4
#define q1 PIN_C5
#define q2 PIN_C6

tADC *a0;
tADC *a1;

unsigned char cmd;
unsigned char val;

#define MSCALE(m) (m/255.0)

void setm(tServo *s, tPin p, float value) {
    if (value < 0.501 && value > 0.499) {
        GPIOPinTypeGPIOInput(PORT_VAL(p), PIN_VAL(p));
    } else {
        GPIOPinTypeGPIOOutput(PORT_VAL(p), PIN_VAL(p));
    }

    SetServo(s, value);
}

void blink(void) {
    static tBoolean led = false;
    SetPin(PIN_F2, led);
    led = !led;
}

void kill(void) {
    setm(m0, p0, 0.5);
    setm(m1, p1, 0.5);
    setm(m2, p2, 0.5);
    setm(m3, p3, 0.5);

    setm(c0, q0, 0.5);
    setm(c1, q1, 0.5);
    setm(c2, q2, 0.5);
}

float getheight(void) {
    static float height = 0.5;
    height = height*0.9 + ADCRead(a0)*0.1;
    return height;
}

void moveup(void) {
    setm(c0, q0, 0.0);

    while (getheight() > 0.385)
        ;

    setm(c0, q0, 0.5);
}

void movemid(void) {
    if (getheight() < 0.42) {
        setm(c0, q0, 1.0);
        while (getheight() < 0.42)
            ;
    } else {
        setm(c0, q0, 0.0);
        while (getheight() > 0.42)
            ;
    }

    setm(c0, q0, 0.5);
}

void movetool(void) {
    setm(c0, q0, 1.0);

    while (getheight() <= 0.470)
        ;

    setm(c0, q0, 0.5);
}

void movedown(void) {
    setm(c0, q0, 1.0);

    while (getheight() <= 0.93)
        ;

    while (getheight() > 0.92)
        ;

    setm(c0, q0, 0.5);
}

void armup(void) {
    setm(c2, q2, 0.65);

    while (ADCRead(a1) < 0.445)
        ;

    setm(c2, q2, 0.5);
}

void armdown(void) {
    setm(c2, q2, 0.35);

    while (ADCRead(a1) > 0.125)
        ;

    setm(c2, q2, 0.5);
}



// The 'main' function is the entry point of the program
int main(void) {
    int i;

    InitializeMCU();

    Printf("Geky's hack enabled\n");
    CallEvery(blink, 0, 0.25);

    m0 = InitializeServo(PIN_D0);
    m1 = InitializeServo(PIN_D1);
    m2 = InitializeServo(PIN_D2);
    m3 = InitializeServo(PIN_D3);

    c0 = InitializeServo(PIN_C4);
    c1 = InitializeServo(PIN_C5);
    c2 = InitializeServo(PIN_C6);

    a0 = InitializeADC(PIN_E1);
    a1 = InitializeADC(PIN_E2);

    for (i = 0; i < 20; i++) {
        getheight();
    }

    kill();

    while (1) {
        cmd = Getc();
        val = Getc();

        if (Getc() != '\n') {
            kill();
        }

        switch (cmd) {
            case 'x':
            kill();
            Printf("c\n");
            break;

            case 'a':
            setm(m0, p0, MSCALE(val));
            Printf("c\n");
            break;

            case 'b':
            setm(m1, p1, MSCALE(val));
            Printf("c\n");
            break;

            case 'c':
            setm(m2, p2, MSCALE(val));
            Printf("c\n");
            break;

            case 'd':
            setm(m3, p3, MSCALE(val));
            Printf("c\n");
            break;

            case 'm':
            moveup();
            Printf("c\n");
            break;

            case 'n':
            movedown();
            Printf("c\n");
            break;

            case 'o':
            movemid();
            Printf("c\n");
            break;

            case 'p':
            movetool();
            Printf("c\n");
            break;

            case 'q':
            armup();
            Printf("c\n");
            break;

            case 'r':
            armdown();
            Printf("c\n");
            break;

            default:
            Printf("x\n");
        }
    }
}
