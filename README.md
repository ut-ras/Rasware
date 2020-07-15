# Rasware
Rasware is a generic robotics library project for the EK-LM4F120XL Stellaris Launchpad, built on top of StellarisWare. It is intended to be used for UT RAS's Robotathon competition as well as general purpose robotics projects.
Authored by the IEEE - Robotics and Automation Society Student Branch at the University of Texas at Austin. For documentation, see Rasware's Github [wiki](https://github.com/ut-ras/Rasware/wiki).

---
# Robotathon Setup Instructions
## Get started with Git
1. Create a free GitHub account on [github.com](https://github.com/signup/free)
3. If you want to learn more about Git check out this great beginners guide http://rogerdudler.github.io/git-guide/ or if you really want to get into it, you can read the GitPro book online for free (and legally, too!) at http://git-scm.com/book
## Fork Rasware (only one Robotathon team member should do this step)
1. Once logged into GitHub, go to [ut-ras](https://github.com/ut-ras)'s [Rasware](https://github.com/ut-ras/Rasware).
2. [Fork](https://help.github.com/articles/fork-a-repo) the project's code through the button on the top right. This will copy the project to your own account.
3. On the team's forked repository Github page, go to `Settings -> Collaborators` and add each team member as a collaborator

The remainder of the instructions will depend on what type of system you're running. 

---
## Recommended (Ubuntu VM on VirtualBox)
We have revived RASBox, which is a virtual image already setup and tailored for your Robotathon needs.
### Install VirtualBox
1. Download and install [VirtualBox](https://www.virtualbox.org/wiki/Downloads) for your specific OS.
2. Download the VirtualBox Extension Pack, also on the same page as the VirtualBox dl.
> **NOTE** If the link above doesn't work, try from Oracle's [site](https://www.oracle.com/virtualization/technologies/vm/downloads/virtualbox-downloads.html).
3. Download the RASBox VM from [Google Drive](https://drive.google.com/file/d/1w93RLiauBfxyeFN1s5DmGF2VfKqs-5xN/view?usp=sharing) (2.1GB).
4. Open VirtualBox and go to File > Preferences > Extensions. Add and install the Extension Pack.
5. Import the VM by going to Import and select the .ova file.
6. Click Next and Import. This might take a couple minutes to complete.
> **NOTE** You may need to enable hardware virtualization from your BIOS. See [here](https://bce.berkeley.edu/enabling-virtualization-in-your-pc-bios.html) for help.
7. Go into Settings after the VM has been imported and check to see if any warnings appear. This may be because too many virtual processors or RAM has been allocated to the machine. Adjust as necessary.
    * By default, only 4GB of RAM and 1 processor is in use. The VM will use up to 20 GB of storage, based on your utilization.
8. Start the VM.

### Forward USB Ports
1. Plug in a Launchpad to your computer. If you don't have a Launchpad available, this step can be done later.
2. When the VM is powered off, go to Settings > USB and check to see if the USB with the + icon when clicked upon shows a device for Texas Instruments In-Circuit Debug Interface.
3. If it does not, and you are running Linux/Mac, add yourself to the vboxusers group `sudo usermod -aG vboxusers <USERNAME>`, replacing `<USERNAME>` with your username. You can check who you are with the command `whoami`.
4. Check to see if you're in the group using `groups <USERNAME>`, if vboxusers shows up you're good and should retry step 2.
5. If step 2 continues to fail, restart your computer to enable changes and try again.
6. If you continue to have issues, try consulting [this thread](https://forums.virtualbox.org/viewtopic.php?f=2&t=79254).
7. You can see if your USB device is recognized after starting the VM, running the VM setup script, and checking `/dev/` for a device called `lm4f`.

### Setting up your machine
Once you've followed these steps to a T, start up and log in to the virtual machine (It's preconfigured to log in without the password by default.) The terminal application should start up and ask you to run the post-install script. Once this is completed successfully, you need to setup RASWare. You need to clone your forked repository of Rasware, and then run that setup script. When complete, you should be able to flash RASDemo to your TM4C!
```sh
# post-install script
cd ~/RASBox
sh setup_vm.sh

# rasware setup script
git clone <URL> # <URL> being the git url of your forked Rasware repo
cd ~/RASBox
sh setup_rasware.sh

# Further development instructions can be found at:
code ~/RASBox/RASbox_User_Guide.md
code ~/RASBox/Rasware/RASwareCrashCourse.md
```
---
## I want to make my own VM!
We suggest that if you do want to create your own VM from another distrobution, allocate about 15-20 GB of storage and 4GB of RAM.
The relevant instructions can be found [here](https://www.virtualbox.org/manual/UserManual.html#intro-installing).
## I want to use my existing Mac/Linux machine to develop without a VM.
Please jump to the [Setup For Mac](#Setup-for-Mac) or the [Setup For Linux](#Setup-for-Linux) instructions further below.

---
## Setup for Mac
### Install Dependencies
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
### Configure Groups
1. On macOS devices belong to the `wheel` group, so in order to access the devices on your Mac (like the Launchpad) you also need to be part of the `wheel` group. Put the following in a terminal to see what groups you're part of:

  ```bash
      groups
  ```
2. If `wheel` was one of the groups listed, you're good to go and can skip the next step.
3. If not, add yourself to the `wheel` group:

  ```bash
    sudo dscl . append /Groups/wheel GroupMembership $(whoami)
  ```
### Jump to the Rasware Instructions
1. If everything has worked so far, follow the rest of the instructions on how to use Rasware from the [Setup Rasware](#Setup-Rasware) instructions.

---
## Setup for Linux
These instructions are written for use in a terminal (xterm, gterm, kterm, tty1, etc.) and assume that you have already installed, and are familiar with, your favorite text editor. If you have not found a favorite text editor, I recomend you take a look at [Vim](http://www.vim.org), [Emacs](http://www.emacswiki.org/emacs/), and [SublimeText3](http://www.sublimetext.com/).
### Install Dependencies
1. First install the basic dependencies
* Archlinux : `sudo pacman -S git base-devel openocd screen arm-none-eabi-gcc gdb arm-none-eabi-newlib`
* Ubuntu/Debian : `sudo apt-get install git build-essential openocd screen gcc-arm-none-eabi gdb libnewlib-arm-none-eabi`

---
## Setup Rasware
### Setup a Directory
1. Create a directory to work in. This is where we will place everything.

  ```bash
    mkdir ras
    cd ras
  ```
### Clone Rasware
Use git to clone Rasware, *replacing <USERNAME> with your Git username in the URL.* Make sure you've forked Rasware already!
This makes a copy of Rasware on your computer.

  ```bash
    git clone https://github.com/<USERNAME>/Rasware.git
  ```
### Download StellarisWare
1. Locate Stellarisware, the TI library, in Rasware/Downloads on your computer (in the repository you cloned).
2. Uncompress the file, compile StellarisWare, and move it to `/usr/local/lib`.

  ```bash
    tar vfx StellarisWare.tar.bz2
    cd StellarisWare
    make
    cd ..
    sudo mv StellarisWare /usr/local/lib
    rm StellarisWare.tar.bz2
  ```
### Add the LM4F rule to UDev
> **NOTE**: skip this step if you're on macOS.
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
> **NOTE**: This may trigger your display to shrink if on a VM. You can fix this by right clicking on the desktop background and rechoosing your display resolution.
3. Plug in a Stellaris Launchpad. you should now see the file `/dev/lm4f`.
### Compile Rasware, RASTemplate, RASDemo
Compile RASLib by using the [make](<https://en.wikipedia.org/wiki/Make_(software)>) program.

  ```bash
    cd Rasware/RASLib
    make
    cd ../RASTemplate
    make
    cd ../RASDemo
    make
  ```
### Compile and run RASDemo
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
  You may get an "Error 1" the first time you run this. In this case, simply make flash again. If this doesn't help, next make sure your USB device is connected, powered on, and forwarded to your VM if necessary. If you see "shutdown command invoked," press the reset button. This will start the program on your launchpad. If you keep getting Error 1's, make sure you've forwarded your LaunchPad's USB connection to the VM.
  
  Else, if at this point an error message is printed that includes "Error erasing flash with vFlashErase packet", run the following command twice and press the board's reset button:
  ```bash
    openocd -f $(find /usr -path */scripts/board/*tm4c123* 2>/dev/null) -c init -c halt -c "flash write_image erase RASDemo.out" -c verify_image RASDemo.out -c halt -c shutdown
  ```
  You should now be able to use `make flash` normally until you flash from Keil again. Keil seems to break things. Thank you to Kevin George for this workaround.
4. If a launchpad is plugged in, it should be accessible at a special file in `/dev` (`/dev/lm4f` on Linux, `/dev/tty.usbmodem[board's serial id]` on macOS - run the detect-board script in the RASLib folder if you're curious). You can use make to create a terminal over [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter).
  ```bash
    make uart
  ```
5. You should now be presented with the RASDemo menu. Feel free to mess around and look into RASDemo's source code to see how it is done. To exit, you can detach the screen by pressing `C-a d`, or control-A followed by a d. To get back to the launchpad's console, in your terminal run `screen -r`. If you would rather kill the screen process, you can press `C-a k y`, or control-A followed by k then y.
6. If you don't want to do all of the above steps each time, you can run everything with one command.
  ```bash
    make run
  ```
  However, please note opening the console may give an error if the command was already run before and was detached. If this happens, try running `screen -r` in console.

  There's also `make start` which will flash your board and start your program _without_ opening the console.

---
## Committing code back to your repo
1. Before you start, you should configure git with both your username and email.
  ```bash
    git config --global user.name "User Name"
    git config --global user.email "your@email.here"
  ```
2. Git will keep track of any changes to your directory. You can use the status command to show the state of git.
  ```bash
    git status
  ```
3. To stage a file for commiting, you can use the add command.
  ```bash
    git add filename
    git add -all  # stage all files
    git add -u    # only modified and deleted files
  ```
4. You can now use the commit command to create a commit containing all of the modifications of the files you `git add`ed. It will open an editor for writing a [commit message](https://github.com/erlang/otp/wiki/Writing-good-commit-messages). If saved, the commit will be created.
  ```bash
    git commit    # this opens up your default text editor
    git commit -m "Commit messge"   # this doesn't open up your editor
  ```
5. To move your local changes to the server, use the push command.
  ```bash
    git push origin master
  ```

