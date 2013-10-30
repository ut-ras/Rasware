#define InitializeMotor0(b1, b2) InitializeMotor(PIN_B7, PIN_B6, (b1), (b2))
#define InitializeMotor1(b1, b2) InitializeMotor(PIN_E5, PIN_E4, (b1), (b2))

#define InitializeServo0() InitializeServo(PIN_B0)

#define InitializeEncoder0(b) InitializeEncoder(PIN_D0, PIN_D1, (b))
#define InitializeEncoder1(b) InitializeEncoder(PIN_D2, PIN_D3, (b))

#define InitializeGPIOLineSensor0() InitializeGPIOLineSensor(PIN_B5, PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_E0, PIN_C6, PIN_C7)

#define InitializeADC0() InitializeADC(PIN_D0)
#define InitializeADC1() InitializeADC(PIN_D1)
#define InitializeADC2() InitializeADC(PIN_D2)
#define InitializeADC3() InitializeADC(PIN_D3)
