Rasware2013
===========

Description
-----------

This is a generic robotics library and demo project for the EK-LM4F120XL Stellaris Launchpad, built on top of Stellarisware. It is intended to be used for our 2013 Robotathon as well as general purpose robotics projects within our organization.
Authored by the IEEE - Robotics and Automation Society Student Branch at the University of Texas at Austin. For documentation, see Rasware2013's Github [wiki](https://github.com/ut-ras/Rasware2013/wiki).

Robotathon Setup Instructions
-----------------

### Get started with Git ###

1. Create a free GitHub account on [github.com](https://github.com/signup/free)
3. If you want to learn more about Git, you can read the GitPro book online for free (and legally, too!) at http://git-scm.com/book

### Fork Rasware (only one Robotathon team member should do this step) ###

1. Once logged into GitHub, go to [ut-ras](https://github.com/ut-ras)'s [Rasware2013](https://github.com/ut-ras/Rasware2013).
2. [Fork](https://help.github.com/articles/fork-a-repo) the project's code through the button on the top right. This will copy the project to your own account.
3. On the team's forked repository Github page, go to Settings->Collaborators and add each team member as a collaborator

The remainder of the instructions will depend on what type of system you're running.

Setup for Windows (WIP)
-----------------

### Install TortiseGit ###

1. Go to [Google Code](https://code.google.com/p/tortoisegit/wiki/Download)
2. Download the correct architecture according to your machine (If you don't know, Right Click in My Computer, click Properties, and check wht System Type is)
3. Run the executable with default values

### Install Stellarisware ###
1. Go to [TI](https://myportal.ti.com/portal/dt?provider=TIPassLoginSingleContainer&lt=myti&j5=2&j3=1&goto=https://my.ti.com/cgi-bin/home.pl) and create a new account.
2. Go to [TI](http://www.ti.com/tool/sw-lm3s), and download SW-LM3S-LM4F.exe.
3. When complete, run the installer with the default installation paths.

### Install Keil uVision ###
1. Go to [TI](http://www.ti.com/tool/SW-EK-TM4C123GXL) and download EK-TM4C123GXL-KEIL.exe.
2. Extact the archive
3. Navigate to Tools/Keil/ and run the installer.

### Install drivers for the Launchpad ###
Note: for windows 8, the drivers are unsigned, so installing them requires you to activate special permissions. Basically follow this process: http://www.tabletroms.com/forums/visual-land-tablets/5894-how-install-unsigned-drivers-windows-8-a.html. (Or else the Device Manager just won't let you install drivers.)

1. Plug in a Stellaris Launchpad to your computer with a usb cord.
2. Windows should fail at installing divers.
3. Go to the Device Manager on your computer.
4.For each listing with a yellow warning, right-click and select `Update Driver`.
5. Click `Browse my Computer` and navigate to the unzipped archive folder.
6. Once all of the drivers are installed, not the COM port number that appears in the Device manager. It should read `Stellaris Virtual Serial Port (COM__)`.

### Install PuTTY ###
1. Go to <http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html>.
2. Under "for Windows on Intel x86" click on [putty.exe](http://the.earth.li/~sgtatham/putty/latest/x86/putty.exe) to save putty to your computer.

### Download Code ###
1. Choose a directory to work in
2. Right click on directory, and select Git Clone
3. Enter your forked repo's url, and press enter

### Compile and run RASDemo ###
1. Open Rasware2013/RASDemo/RASDemo.uvproj with Keil uVision. This is an example project we made to demostrate how to get a lot of useful peripherals working with the Launchpad, like motors and line sensors. 
2. With the Launchpad plugged into your computer, in Keil do Project->Build Target and then Flash->Download. This compiles and loads the project onto the Launchpad.
3. Open PuTTY and connect to the board 
    1. Click the "serial" radio button
    2. Under "serial line" type "COM#" where # was the COM port number shown in the Device Manager 
    3. Under "speed" type "115200"
    4. Under "Saved Settings" type "Rasware2013" and then click "Save". 
    5. Now the settings are saved in PuTTY. In the future, just open PuTTY and double click "Rasware2013" under "Saved Settings".
[TODO: finish this section once RASDemo is done]

### Starting your own project ###
[TODO: Make a starter project for RASLets to use?]

### Committing code back to the repo ###
1. While in the directory, right click and select Git Commit
2. Write useful commit comments. See [link](https://github.com/erlang/otp/wiki/Writing-good-commit-messages). Your teammates will thank you.
3. You will be promted to do a Git Push. Do so. At this point, you've only made changes to the repo on your machine. The Git Push uploads the changes to GitHub.
4. If you need to log in, check with a mentor to resetup 

Setup for Linux (WIP)
---------------

Just a quick warning: unlike Linux, Windows is actually supported by TI for the LM4F. The Linux support is entirely supplied by hobbiest who have put together various toolchains for others to use. This means that a Windows setup will be much easier to create. However, Linux has many benefits, especially for coders, so these instructions are here for those who are already experienced with Linux and willing to take the challenge.

For these instructions, an Ubuntu install is assumed. It should be able to work on any Linux system, and has been tested on Arch and Debian. If you are using a different package manager than apt-get, things should still work if you substitute the correct command.

### Setup a directory
1. Create a directory to work in. This is where we will place everything.
    1. "mkdir ras"
    2. "cd ras"

### Install Git ##
1. Simply install from your distro.
    1. "sudo apt-get install git"
2. You should configure git with both your username and email.
    1. "git config --global user.name "User Name"
    2. "git config --global user.email "your@email.here"

### Installing a cross compiler ###
1. Cross Compilers for the LM4F can be found [here](https://launchpad.net/gcc-arm-embedded)
2. Download and move to your ras directory.
3. Uncompress the file.
    1. "tar vfxj gcc-arm-none-eabi\*.bz2"
    2. "rm gcc-arm-none-eabi\*.bz2"
    3. "mv gcc-arm-none-eabi\* gcc-arm-none-eabi"
4. You should add the cross-compiler to your path. To do this append the following line to your ~/.bashrc file. Make sure to change the path to the correct location of the ras directory.
    1. "export PATH=$PATH:~/ras/gcc-arm-none-eabi/bin"
5. You can then run gcc. It should complaign about missing files, but that will prove that it works.
    1. "source ~/.bashrc"
    2. "arm-none-eabi-gcc"

### Install StellarisWare ###
1. Download [StellarisWare](http://ras.ece.utexas.edu/drivers/StellarisWare.zip), the TI library and move it to your ras directory.
2. Uncompress the file and compile StellarisWare
    1. "unzip StellarisWare.zip -d StellarisWare"
    2. "cd StellarisWare"
    3. "make"

### Install LM4Flash ###
1. The lm4flsh utility allows us to program the board. To obtain the program you can simply download it from the [github repo](https://github.com/utzig/lm4tools).
    1. "git clone https://github.com/utzig/lm4tools.git"
2. You will need to compile the library before you can use it.
    1. "cd lm4tools/lm4flash"
    2. "make"
3. If there is an error due to libusb, make sure the package is installed.
    1. "sudo apt-get install libusb"
4. You should also add the lm4flash to your path. To do this append the following line to your ~/.bashrc file. Make sure to change the path to the correct location of the ras directory.
    1. "export PATH=$PATH:~/ras/lm4tools/lm4flash"

### Download Code ###
1. You can now use git to create a copy of your forked repo with the clone command.
    1. "git clone https://github.com/username/Rasware2013.git"


### Compile and run RASDemo ###
1. Open Rasware2013/RASDemo/RASDemo.uvproj with Keil uVision. This is an example project we made to demostrate how to get a lot of useful peripherals working with the Launchpad, like motors and line sensors. 
2. With the Launchpad plugged into your computer, in Keil do Project->Build Target and then Flash->Download. This compiles and loads the project onto the Launchpad.
3. Open PuTTY and connect to the board 
    1. Click the "serial" radio button
    2. Under "serial line" type "COM#" where # was the COM port number shown in the Device Manager 
    3. Under "speed" type "115200"
    4. Under "Saved Settings" type "Rasware2013" and then click "Save". 
    5. Now the settings are saved in PuTTY. In the future, just open PuTTY and double click "Rasware2013" under "Saved Settings".
[TODO: finish this section once RASDemo is done]

### Starting your own project
[TODO: Make a starter project for RASLets to use?]

### Committing code back to the repo
1. While in the directory, right click and select Git Commit
2. Write useful commit comments. See [link](https://github.com/erlang/otp/wiki/Writing-good-commit-messages). Your teammates will thank you.
3. You will be promted to do a Git Push. Do so. At this point, you've only made changes to the repo on your machine. The Git Push uploads the changes to GitHub.
4. If you need to log in, check with a mentor to resetup 
