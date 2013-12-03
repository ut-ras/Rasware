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

These instructions are written for use in a terminal (xterm, gterm, kterm, tty1, etc.) and assume that you have already installed, ard are farmiliar with, your favorite text editor. If you have not found a favorite text editor, I recomend you take a look at [Vim](http://www.vim.org), [Emacs](http://www.emacswiki.org/emacs/), and [SublimeText2](http://www.sublimetext.com/2).

### Setup a Directory  ###
1. Create a directory to work in. This is where we will place everything.

        mkdir ras
        cd ras

### Install Dependencies ###
1. First install the basic dependencies
 * Archlinux : `sudo pacman -S git screen libusb`
 * Ubuntu/Debian : `sudo apt-get install git screen libusb-dev pkg-config build-essential`
2. If your computer is 64-bits, you will also need to install `ia32-libs`
3. If libusb-dev is not available, you may be able just use `libusb`

### Install the Cross Compiler ###
1. Cross Compilers for the LM4F can be found [here](https://launchpad.net/gcc-arm-embedded)
2. Download and move to your ras directory.
3. Uncompress the file and install it your your /usr/local directory.

        tar vfxj gcc-arm-none-eabi*.bz2
        sudo cp -r gcc-arm-none-eabi*/bin/* /usr/local/bin
        sudo cp -r gcc-arm-none-eabi*/lib/* /usr/local/lib
        sudo cp -r gcc-arm-none-eabi*/share/* /usr/local/share
        sudo cp -r gcc-arm-none-eabi*/arm-none-eabi /usr/local
        rm -r gcc-arm-none-eabi*

4. You should now be able to run the compiler. It should complain about missing input files, but that means that just means the compiler is working.

        arm-none-eabi-gcc

### Install LM4Flash ###
1. The lm4flsh utility allows us to program the board. To obtain the program you can download it from the [github repo](https://github.com/utzig/lm4tools).

        git clone https://github.com/utzig/lm4tools.git

2. You will need to compile the library before you can use it.

        cd lm4tools/lm4flash
        make

3. Now just move it to local/bin so it is in your path.

        sudo mv lm4flash /usr/local/bin
        cd ../..
        rm -rf lm4flash

### Download the Cortex-M4 Libraries ###
1. Due to an issue in the ARM libraries provided by the cross-compiler, you will need to download the [correct libraries](http://toast.projectgeky.com/rasware/CortexM4Libs.tar.bz2) for the Cortex-M4.
2. Move the libraries to your ras directory and uncompress the file.

        tar vfxj CortexM4Libs.tar.bz2

### Download StellarisWare ###
1. Download [StellarisWare](http://toast.projectgeky.com/rasware/StellarisWare.tar.bz2), the TI library and move it to your ras directory.
2. Uncompress the file and compile StellarisWare.

        tar vfxj StellarisWare.tar.bz2
        cd StellarisWare
        make
        cd ..
        rm StellarisWare.tar.bz2

### Download Rasware ###
1. You can now use git to create a copy of your forked repo with the clone command.

        git clone https://github.com/username/Rasware2013.git

2. Before you use it, make sure to compile RASLib.

        cd Rasware2013/RASLib
        make

### Add the LM4F to UDev ###
1. To keep from needing root access to communicate with the lm4f, you will need to copy the lm4f rule to the udev directory.

        sudo cp Rasware2013/RASLib/51-lm4f.rules /etc/udev/rules.d

2. Have udev re-read it's rules for these changes to come into effect.

        sudo udevadm control --reload
        sudo udevadm trigger

3. Plug in a Stellaris Launchpad. you should now see the file /dev/lm4f appear.

### Compile and run RASDemo ###
1. Like most projects on Linux, Rasware can be compiled with make. We have created an example project to demonstrate how to use several useful peripherals with the Launchpad, like motors and line-sensors.
2. To compile RASDemo, simply run make.

        cd Rasware2013/RASDemo
        make

3. The lm4flash utility can be used to load your program on the board. There is a slight issue in Debian that may require running the command with sudo.

        lm4flash RASDemo.axf

4. The special file /dev/lm4f can now be written to and read from to send data over uart. You can now use an external program, such as screen, to communicate with the devices.

        screen /dev/lm4f 115200

5. You should now be presented with a menu for using RASDemo. Feel free to mess around and look into RASDemo's source code to see how it is done. To exit press `C-a k y`, or control-A followed by a K followed by a Y.
6. If you don't want to do all of the above steps by hand each time, you can make the code, flash the board, and start screen all by running the command `make run`.

### Starting your own project
[TODO: Make a starter project for RASLets to use?]

### Committing code back to the repo
1. Before you start, you should configure git with both your username and email.

        git config --global user.name "User Name"
        git config --global user.email "your@email.here"

2. Git will keep track of changes to your directory. You can use the status command to show the state of git.

        git status

3. To track a file, use the add command

        git add filename

4. You can now use the commit command to create a commit which contians all of the modifications to files you `git add`ed. It will open an editor, which defaults to VIM, for writing a [commit message](https://github.com/erlang/otp/wiki/Writing-good-commit-messages). If saved, the commit will be created. To change the default editor, assign a program to the EDITOR variable

        git commit

5. To move your local changes to the server, simply use the push command.

        git push origin master

