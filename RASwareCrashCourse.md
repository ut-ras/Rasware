# RASware Crash Course
RASTemplate is a template of a basic program that you can use to immediately start programming instead of having to deal with Makefile setup.



## Step 0: Make sure you have RASware set up!
If you don't, go through the [installation instructions](https://github.com/ut-ras/Rasware) here!

If you still need help, find a mentor who specializes in whatever you need and go to their [office hours](https://docs.google.com/spreadsheets/d/1pGwM__kKSpHq0Mno_v-tEsfJyLduxxpoXtd1iVQccQI/edit?usp=sharing)!

## Step 1: Copy RASTemplate.
We recommend you make a copy of the RASTemplate folder so you have a clean copy whenever you need it. For this tutorial, we'll call the folder Robotathon17.

Open your terminal and navigate to your Rasware folder. Then, make your copy:

```bash
../../Rasware $ cp -r RASTemplate/ Robotathon17/
```
Change into your new folder:
```bash
../../Rasware $ cd Robotathon17
```
Open up Makefile with your favorite text editor. It'll look something like this:
```c
# Makefile for RASWare 2013
TARGET = RASTemplate.out
...
```
Change that RASTemplate.out to whatever you named your project folder (in this case, Robotathon17.out). 

Save the file, go to your terminal, and do a ```make clean``` followed by a ```make``` to clean out your old binaries and build some new ones .

## Step 2: Look at your Main.c.

Using your favorite text editor, open up the Main.c file from your Robotathon17 folder. Let's look at a few things:

* All of the ```#include``` statements at the top are your included header files. A header (.h) file contains only function prototypes. By including this file, you gain access to all of the functions that are defined in the corresponding source (.c) file, which **must** have the same name as the header. We'll go over header and source files in a little bit. Included in Main.c are:
  * common.h, which must be included in all RASware files.
  * gpio.h, which gives access to all of the General Purpose Input/Output pins on the Launchpad.
  * time.h, which gives us time-related functions.
* ```tBoolean blink_on``` is a good example of a couple things:
  * A boolean variable type.
  * A global variable with initial value ```true```. You should put all of your global variables here (global variables are accessible by every function in the file; as opposed to local variables, defined inside a function, which are only accessible in the scope of that function).
* ```void blink(void)``` is a function header. The first ```void``` is the return type; if this is anything other than void, the function will return that type of variable when it is finished executing. Void returns nothing. ```blink``` is the name, and the second ```void```, in parentheses, is the parameter list. This function takes no parameters, but if it did, we would list out the parameters the function needs to work along with their types. Parameters are local to the function.
  * The ```blink()``` function is simple: it sets ```PIN_F3```, the onboard green LED, to the value of ```blink_on```, which it then toggles so the next iteration will toggle the LED. 
  * **Try this:** Change ```PIN_F3``` to ```PIN_F2```. This will cause the function to toggle the onboard blue LED after you flash the board, indicating that you're running a different program than before.
* ```int main(void)``` is the main function, which is the entry point of any program. You see that we first set up a callback with CallEvery that calls ```blink()``` every 0.5 seconds (look at ```RASLib/inc/time.h``` for the exact parameter list!). Then, we go into an infinite loop (which is desirable if we want our robot to stay alive) and repeatedly print "Hello World!". 

If you made any changes, save your Main.c, then open your terminal and in the Robotathon17 folder, run the following with a Launchpad connected to your computer (and forwarded to your VM, if you're running a VM):

```bash
make
make flash
make flash
make uart
```

The first ```make flash``` will return an Error 1; this is fine. Run the second one, then when you see ```shutdown command invoked```, press the Launchpad's reset button (notice the blue LED start flashing if you made the change above) before running ```make uart```, which will open up a screen to see what the Launchpad is printing. To kill the screen, simply type ```Ctrl+A``` ```k``` ```y```. 



## Step 3: Which pins should I use? 

Here are a few things to keep in mind when choosing pins to wire your components to:

* All of the pins you can use have the type ```tPin```, and the format for how you can reference them in code are listed in ```RASLib/inc/gpio.h```. It's easy though - assuming you want to use Pin D0, it's just ```PIN_D0```.
* There is one typo on the Launchpad itself. ```PIN_E0``` refers to the pin labeled ```P30``` on the front of the Launchpad. 
* Ports A, B, D, and E are all pretty safe to use. Avoid using Port C because it connects to the Launchpad's JTAG programming header, and avoid Port F because:
  * PF0 is connected to SW2 (negative logic).
  * PF1 is the onboard red LED.
  * PF2 is the onboard blue LED.
  * PF3 is the onboard green LED.
  * PF4 is connected to SW1 (positive logic).
* The Plague Resistors:
  * If you ever plan to use PD0 and PB6 at the same time, remove resistor R9.
  * If you ever plan to use PD1 and PB7 at the same time, remove resistor R10.


## Step 4: Hardware Connections.

### To power your Launchpad from battery:

Connect the clean 5V from your voltage regulator's VOUT to the Launchpad's VBUS. Connect the regulator's GND to the Launchpad's GND. Flip the switch at the upper left of the Launchpad to the **left**, where it says DEVICE. This will isolate the Launchpad's power rail from the USB, but maintain a data connection. **You need to have this switch at DEVICE and power from a battery whenever you are connected to both USB and motors,** or else the motors will attempt to draw too much current from your USB port and it will shut down the port. Likewise, **have the switch at DEVICE whenever your battery is connected to your Launchpad,** or else your batteries will attempt to charge from USB and again draw too much current. If you are only testing sensors and not motors, you can disconnect your battery and power your Launchpad from USB by moving the switch to DEBUG. **ALWAYS POWER OFF YOUR LAUNCHPAD BEFORE CONNECTING OR DISCONNECTING ANY COMPONENTS!**

### Battery

*NOTE: male/female refers to the XT-60's metal terminals, NOT the plastic housing!*

Take your battery, your 2-terminal rocker switch, and your **female** blue power connector. Solder the red wire coming from your battery snaps to one terminal on the rocker switch. Solder a wire from the other switch terminal to the + side of your female blue power connector. Solder the black wire from your battery snaps to the - side of your female blue power connector. Take your **male** blue power connector, and solder a red wire to + and a black wire to -. Connect the male and female power connectors. Once you plug the wires coming from your male connector into your voltage regulator, you can use the switch to turn your battery on and off. To charge your battery, disconnect your male connector and plug your battery's female connector into the charger's male connector.

### Voltage Regulator

First, solder on your 5-pin header, then place the regulator on a breadboard so each pin has its own node. Take the **red** wire from your male blue power connector and plug it into VIN. Plug the **black** wire from your male blue power connector into GND. Once you power your battery up, you'll have a clean 5V at 5A power supply at the VOUT pin, and ground at GND.

### Bumper Switch (Touch Sensor)

**First**, take a multimeter and make sure you know which of the three terminals is **common**, which of the three terminals is connected to common when the switch is **open**, and which of the three terminals is connected to common when the switch is **closed**. Bring your +3.3V signal from your Launchpad to the breadboard (**DO NOT** connect this to the VOUT of your regulator - put it at a different node), as well as connect your Launchpad's GND to your power regulator's GND (your entire system must have a common ground). To wire a positive logic switch, connect your **open** terminal to GND, your **closed** terminal to +3.3V, and your **common** terminal to a Launchpad pin. Note the pin, we'll use it when writing code later.

### Infrared Distance Sensor

The IR sensor has three wires:

* Red is voltage. Connect it to +3.3V.
* Black is ground. Connect it to GND.
* White is signal. Connect it to one of the pins specified in ```RASLib/inc/adc.h```. Note which pin you choose!

If you don't have the three-wire connector with a white plactic terminal to plug into the IR sensor, the signals are as follows. With the sensor pointed towards you with the white terminal on the top:

* +3.3V is on the left.
* Ground is in the center.
* Signal is on the right. 

You can cut the white terminal and solder wires directly to the sensor's three contacts. 

### Line Sensor (Reflectance Sensor Array)

You need 11 wires to connect your line sensor:

* VCC should go to +3.3V.
* LED ON should go to +3.3V. 
* You only need to connect one of the GND wires to your GND node. 
* You'll need to connect your eight signal wires, labeled 1-8 on the line sensor, to 8 pins on the board. I recommend using the bottom eight pins on header J1: PB0, PB1, PE4, PE5, PB4, PA5, PA6, PA7. The order you connect these is very important, so make sure your wires don't cross while you're making connections!


### Continuous Servo Motor

**Remember to power your Launchpad from your battery for this!** The continuous servos have three wires:

* Red is voltage. Connect this to the 5V coming from the VOUT of your regulator.
* Black is ground. Connect it to the GND of your regulator.
* White is signal. Connect it to a Launchpad pin and note the pin for later.

### Micro Servo Motor

**Remember to power your Launchpad from your battery for this!** The micro servos have three wires, which may be different colors than your standard red-black-white:

* Voltage is usually red, and is typically the center wire. Connect this to the 5V at VOUT of your regulator.
* Of the two remaining wires, the darker is usually ground. Connect this to the GND of your regulator.
* The remaining wire is signal. Connect this to a Launchpad pin and note the pin for later.

We recommend finding the datasheet for your micro servo and verifying these wires.

## Step 5: Software.

### Bumper Switch (Touch Sensor)

We'll use this code example to explain how to create new source and header files. 

Create a new file in your Robotathon2017 project called ```Switch.c```. In this file, you'll need to include ```RASLib/inc/common.h``` and ```RASLib/inc/gpio.h```. 

Create a global ```tBoolean value``` with initial value ```false```. 

Open up ```RASLib/inc/gpio.h``` and think about which functions you'll need. You need to initialize the GPIO and get the value of a pin.

Create a function with void return and void parameters called ```InitSwitch```. In the body of this function, call ```InitializeGPIO()```. 

Create another function with void return and a ```tPin``` parameter called ```pin``` . Call the function ```PrintSwitch```, as all it will do is print the value of a pin, 1 or 0. In this function, set ```value``` equal to a call to ```GetPin(pin)```. This reads the value of the pin you got as a parameter. Then print out the value of the ```value```. 

Your entire ```Switch.c``` will look like this:

```c
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>

tBoolean value = false;

void InitSwitch(void) {
    InitializeGPIO();
}

void PrintSwitch(tPin pin) {
    value = GetPin(pin);
  	Printf("Switch value is %d\n", value); //%d is the escape character for integers
  	// \n is the escape character for newlines
}

```

Save your ```Switch.c```. Then, create your header file, ```Switch.h```. All you need to do in ```Switch.h``` is declare the function prototypes of the functions you need to access from another file. Your ```Switch.h``` should look like this:

```c
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>

void InitSwitch(void);
void PrintSwitch(tPin pin);

```

Save ```Switch.h```. Now open up your ```Main.c```. At the top, add an include statement: ```include "Switch.h"```. This is in quotes and not angled brackets because it is a local file in the same folder as ```Main.c```, not somewhere else. Then, in your main function, outside of the while loop, call ```InitSwitch()```. Inside of your while loop, call ```PrintSwitch(PIN_xx)``` with whatever pin you connected your bumper switch to. Save your files, and then to flash and open a screen to see the switch value change as you press it, enter the following in your terminal while in the Robotathon17 folder:

```bash
make
make flash
make flash
make uart
```

### Infrared Distance Sensor

You can write all of the code for this in ```Main.c``` . To see the functions you'll need, examine ```RASLib/inc/adc.h```. This is the file you'll need to include as well! Any RASLib header files are included in angled brackets because the are located in a different folder than your ```Main.c``` .

In your main function before your while loop, initialize an ADC pointer, as well declare a float value to store the pin value:

```c
tADC *dist = InitializeADC(PIN_xx);
float distval;
```

In your while loop, read the value of the sensor and print it out:

```c
distval = ADCRead(dist);
Printf("IR sensor value is %f\n", distval); //%f is the escape character for floats
```

Then save, ```make```, flash, and run your code to see your IR sensor values printed out. Experiment with your IR sensor to see what values it returns at known distances using a ruler; each sensor is different, so you'll need to accommodate! Note that the effective range of the Sharp IR sensors we provide is 10cm to 80cm. 

### Line Sensor (Reflectance Sensor Array)

The file to include is ```RASLib/inc/linesensor.h```. 

In your main, in your initialization block, initialize a pointer to a line sensor object and declare an array of eight floats to store the line sensor data:

```c
tLineSensor *line = InitializeGPIOLineSensor(PIN_xx, PIN_xx, PIN_xx, PIN_xx, PIN_xx, PIN_xx, PIN_xx, PIN_xx); 
 //These are the eight pins you chose in the proper order
float linevals[8];
```

In your while loop, read the line sensor values into the array and print them, making sure to specify the floats for printing in the proper order:

```c
LineSensorReadArray(line, linevals);
Printf(“%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t”, value[0], value[1], value[2], 
  value[3], value[4], value[5], value[6], value[7]); 
  // \t is the escape character for tabs
```

Save, ```make```, flash, and run your code to see how the line sensor behaves when you hold it over a white background with a black line. 

### Continuous Servo Motor

The file to include is ```RASLib/inc/motor.h```.

In your main, in your initialization block, initialize your motor pointers, making sure to invert one so your robot doesn't spin in circles:

```c
tMotor *left = InitializeServoMotor(PIN_xx, true);
tMotor *right = InitializeServoMotor(PIN_xx, false);
```

In your while loop, set the motor according to your game logic and sensor input. Here we'll just set one to go full forward and the other full reverse. ```SetMotor```'s second parameter can be a float value on the range [-1,1]. 

```c
SetMotor(left, 1.0);
SetMotor(right, -1.0);
```

Save, ```make```, flash, and run your code to see your motors turn. **Make sure you're powering your Launchpad and motors from your regulator!**

### Micro Servo Motor

The file to include is ```RASLib/inc/servo.h```. 

In your main, in your initialization block, initialize your servo pointer:

```c
tServo *ser = InitializeServo(PIN_xx);
```

In your while loop, set your servo according to how you plan to use it. Here we'll show how to make it tick through a number of positions every half second. ```SetServo``` 's second parameter is a float value on the range [0,1].

```c
float i;
for (i = 0.0; i < 1.0; i += 0.1) {
    SetServo(ser, i);
  	Wait(0.5); //This function is in RASLib/inc/time.h
}
for (i = 1.0; i > 0.0; i -= 0.1) {
    SetServo(ser, i);
  	Wait(0.5);
}
```



## Step 6: Good luck! 

Please come to [office hours](https://docs.google.com/spreadsheets/d/1pGwM__kKSpHq0Mno_v-tEsfJyLduxxpoXtd1iVQccQI/edit?usp=sharing) if you need any help! 

# FIN

