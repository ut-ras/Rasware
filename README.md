Rasware
=======


Description
-----------

Rasware is a generic robotics library project for the EK-LM4F120XL Stellaris Launchpad, built on top of StellarisWare. It is intended to be used for UT RAS's Robotathon competition as well as general purpose robotics projects.
Authored by the IEEE - Robotics and Automation Society Student Branch at the University of Texas at Austin. For documentation, see Rasware's Github [wiki](https://github.com/ut-ras/Rasware/wiki).


Robotathon Setup Instructions
-----------------------------

### Get started with Git ###
1. Create a free GitHub account on [github.com](https://github.com/signup/free)
3. If you want to learn more about Git, you can read the GitPro book online for free (and legally, too!) at http://git-scm.com/book

### Fork Rasware (only one Robotathon team member should do this step) ###
1. Once logged into GitHub, go to [ut-ras](https://github.com/ut-ras)'s [Rasware](https://github.com/ut-ras/Rasware).
2. [Fork](https://help.github.com/articles/fork-a-repo) the project's code through the button on the top right. This will copy the project to your own account.
3. On the team's forked repository Github page, go to `Settings -> Collaborators` and add each team member as a collaborator

The remainder of the instructions will depend on what type of system you're running. 


Setup for Windows (with VMware)
-------------------------------
### Install VMware ###
1. Download and install [VMware Workstation Player](https://my.vmware.com/web/vmware/downloads).
2. WMware Player is a hypervisor that lets us run a controlled environment for writing and flashing code.
3. Download a copy of the latest Ubuntu LTS .iso from [here](https://www.ubuntu.com/download/desktop).
4. Open VMWare and select Create a New Virtual Machine.
5. Select the Installer disc image file bubble and navigate to your freshly downloaded .iso. Click next.
6. Full Name is your own name. User name is whatever you want, all lower case, no special characters. Create and confirm a password. Click next.
7. Choose a name for your VM. (Some people like to theme their Linux machine names. We've seen famous scientists, video game AIs, Star Trek starship classes, whatever you want.)
8. Make sure you're okay with the location where you'll store this machine, then click next.
9. 20GB is a good max disk size. This will not immediately allocate 20GB, it will be populated as you fill it. This is just the max allowed size.
10. Store your virtual disk as a single file. Click next.
11. Click Customize Hardware, then slide the memory slider up to 4GB. This is plenty of RAM for this VM's purposes. Click close.
12. Click finish, let the machine install, then log in!



### Forward USB ###
1. Plug in a Launchpad to your computer. If you don't have a Launchpad available this step can be done later.
2. In the VM, select `Player -> Removable Devices -> Luminary Micro ICDI` and select `Connect`.



### Jump to the Linux Instructions ###
1. Follow the rest of the instructions on how to use Rasware from the [clone rasware](#clone-rasware-1) step in the Linux instructions.


Setup for Mac
-------------

### Install Dependencies ###
0. Before we do anything else, run this in your terminal if you're running Marericks or newer (macOS 10.9+) and haven't already done this:

    ```bash
        xcode-select --install
    ```

1. Install Homebrew: [link](http://brew.sh) (Installation instructions are at the bottom of the page.)
2. Install OpenOCD through Homebrew in a terminal.

  ```bash
    brew install openocd
  ```

3. Install the Cross Compiler Toolchain for Embedded ARM Devices through Homebrew as well:

    ```bash
        brew cask install gcc-arm-embedded
    ```

4. Locate Stellarisware in your Rasware/Downloads, and extract.

### Setup a Directory  ###
1. Create a directory to work in. This is where we will place everything.

  ```bash
    mkdir ras
    cd ras
  ```

### Clone Rasware ###
1. Use git to clone Rasware, replacing "username" with your Git username in the URL. Make sure you've forked Rasware already!

  ```bash
    git clone https://github.com/username/Rasware.git
  ```


### Modify the Makefiles to point to tools ###
1. In Rasware, open up RASLib/Makefile with your favorite text editor and change the line the line containing the `STELLARIS` variable.
  ```make
    STELLARIS = ../../StellarisWare
  ```
  
2. Repeat the above for RASDemo/Makefile and RASTemplate/Makefile.

3. In all three Makefiles, around lines 85-90, there are lines that say "/dev/lm4f". You need to change "lm4f" to be whatever your computer sees your Launchpad as. The easiest way to do this is to unplug the Launchpad, run "ls /dev", plug in the Launchpad, and run "ls /dev" again. Find the device that was added to the list and change the paths on those lines in the Makefiles to match that device. 

4. Compile RASLib.

  ```bash
    cd Rasware/RASLib
    make
  ```


### Jump to the Linux Instructions ###
1. If everything has worked so far, follow the rest of the instructions on how to use Rasware from the [compile and run rasdemo](#compile-and-run-rasdemo) step in the Linux instructions.


Setup for Linux
---------------
These instructions are written for use in a terminal (xterm, gterm, kterm, tty1, etc.) and assume that you have already installed, and are familiar with, your favorite text editor. If you have not found a favorite text editor, I recomend you take a look at [Vim](http://www.vim.org), [Emacs](http://www.emacswiki.org/emacs/), and [SublimeText2](http://www.sublimetext.com/2).

### Setup a Directory  ###
1. Create a directory to work in. This is where we will place everything.

  ```bash
    mkdir ras
    cd ras
  ```


### Install Dependencies ###
1. First install the basic dependencies
* Archlinux : `sudo pacman -S git base-devel openocd screen arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib`
* Ubuntu/Debian : `sudo apt-get install git build-essential openocd screen gcc-arm-none-eabi gdb-arm-none-eabi libnewlib-arm-none-eabi`

### Clone Rasware ###
Use git to clone Rasware, replacing "username" with your Git username in the URL. Make sure you've forked Rasware already! If you're coming from the Max instructions 

  ```bash
    git clone https://github.com/username/Rasware.git
  ```

### Download StellarisWare ###
1. Locate Stellarisware, the TI library, in Rasware/Downloads.
2. Uncompress the file, compile StellarisWare, and move it to `/usr/local/lib`.

  ```bash
    tar vfx StellarisWare.tar.bz2
    cd StellarisWare
    make
    cd ..
    sudo mv StellarisWare /usr/local/lib
    rm StellarisWare.tar.bz2
  ```


### Add the LM4F rule to UDev ###
1. To avoid needing root access to communicate with the lm4f, you will need to copy the lm4f rule to the udev directory.

  ```bash
    wget https://raw.githubusercontent.com/ut-ras/Rasware/master/RASLib/51-lm4f.rules
    sudo mv 51-lm4f.rules /etc/udev/rules.d
  ```

2. Trigger udev for the new rules to come into effect.

  ```bash
    sudo udevadm control --reload
    sudo udevadm trigger
  ```

3. Plug in a Stellaris Launchpad. you should now see the file `/dev/lm4f`.

### Compile Rasware ###
Compile RASLib by using the [make](<https://en.wikipedia.org/wiki/Make_(software)>) program.

  ```bash
    cd Rasware/RASLib
    make
  ```
Repeat for RASTemplate and RASDemo.

### Compile and run RASDemo ###
1. We have created an example project to demonstrate Rasware running on a Launchpad.
2. Compile RASDemo.

  ```bash
    cd Rasware/RASDemo
    make
  ```

3. To flash RASDemo to the board, we use make with different arguments.

  ```bash
    make flash
  ```
  If you get an Error 1, make sure your USB device is connected, powered on, and forwarded to your VM if necessary and try again. If you see "shutdown command invoked," press the reset button. If you get another Error 1, `make flash` again and press reset. If you keep getting Error 1's, make sure you've forwarded your LaunchPad's USB connection to the VM: In the VM, select `Player -> Removable Devices -> Luminary Micro ICDI` and select `Connect`.
  
  Else, if at this point an error message is printed that includes "Error erasing flash with vFlashErase packet", run the following command twice and press the board's reset button:
  ```bash
    openocd -f /usr/share/openocd/scripts/board/ek-tm4c123gxl.cfg -c init -c halt -c "flash write_image erase RASDemo.out" -c verify_image RASDemo.out -c halt -c shutdown
  ```
  You should now be able to use `make flash` normally until you flash from Keil again. Keil seems to break things. Thank you to Kevin George for this workaround.

4. If a launchpad is plugged in, the special file `/dev/lm4f` should be available. You can use make to create a terminal over [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter).

  ```bash
    make uart
  ```

5. You should now be presented with the RASDemo menu. Feel free to mess around and look into RASDemo's source code to see how it is done. To exit press `C-a k y`, or control-A followed by a K followed by a Y.

6. If you don't want to do all of the above steps each time, you can run everything with one command.

  ```bash
    make run
  ```


### Committing code back to your repo ###
1. Before you start, you should configure git with both your username and email.

  ```bash
    git config --global user.name "User Name"
    git config --global user.email "your@email.here"
  ```

2. Git will keep track of any changes to your directory. You can use the status command to show the state of git.

  ```bash
    git status
  ```

3. To track a file, you can use the add command.

  ```bash
    git add filename
  ```

4. You can now use the commit command to create a commit containing all of the modifications of the files you `git add`ed. It will open an editor for writing a [commit message](https://github.com/erlang/otp/wiki/Writing-good-commit-messages). If saved, the commit will be created.

  ```bash
    git commit
  ```

5. To move your local changes to the server, use the push command.

  ```bash
    git push origin master
  ```


### Fin ###

