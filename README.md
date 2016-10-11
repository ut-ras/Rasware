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

The remainder of the instructions will depend on what type of system you're running. If you are running Windows 10, you may have better luck with the [VMware](#setup-for-windows-with-vmware), otherwise we suggest [Virtual Box](#setup-for-windows-with-virtual-box) for Windows users.


Setup for Windows (with Virtual Box)
------------------------------------

### Install Virtual Box ###
1. Download and install [Virtual Box](https://www.virtualbox.org/wiki/Downloads).
2. Oracle's Virtual Box is a hypervisor that lets us run a controlled environment for writing and flashing code.
3. The "Host Machine" refers to your physical computer, so you will want the installer for "Windows Hosts"

### Download the RASBox Image ###
1. Download [RASBox](http://128.83.136.86/rasware/rasbox.ova).
2. Launch Oracle Virtual Box.
3. Import the RASBox Image by selecting `File -> Import Appliance` and navigating to the downloaded file.
4. Once imported, there should be a `rasbox` VM listed in Virtual Box. Select it and click `start` to launch the VM.
5. The user name is `ras` and there is no password.

### Forward USB ###
1. Plug in a Launchpad to your computer. If you don't have a Launchpad available this step can be done later.
2. In the RASBox VM, select `Machine -> Settings` and select the `USB` settings.
3. Click the little USB symbol with the + sign, select the `Texas Instruments In-Circuit Debug Interface` device, and click `OK`.
4. Once selected, replugging in the Launchpad should be detected by the RASBox.

### Install PuTTY (Optional but suggested) ###
1. Download [putty.exe](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html).
2. Putty is an incredibly useful little tool for terminal connections in Windows.
3. In the VirtualBox Manager, select `File -> Preferences`, select `Network`, select `Host-only Networks`, and then select the little screwdriver symbol.
4. Change the IPv4 Address to `20.0.0.1` and hit `OK` until all the windows are gone.
5. Launch Putty and set the hostname to `ras@20.0.0.8` which is the default username and IP address of the RASBox.
6. You may want to also give the session a name and save it.
7. Click `Open` to create an SSH connection to the RASBox.

### Using the Terminal ###
1. You should now be presented with a terminal showing something like `ras@rasbox:~$ `. This is known as a [shell or command line](<https://en.wikipedia.org/wiki/Shell_(computing)>) which is the main interface for using our system.
2. Here is a quick introduction ([part 1](http://www.linfo.org/command_line_lesson_1.html), [part 2](http://www.linfo.org/command_line_lesson_2.html)) in using the shell.
3. To edit files, [nano](https://en.wikipedia.org/wiki/GNU_nano) is suggested for new users. You can edit a file, for example hello.txt below, by typing the following into the shell.

  ```bash
    nano hello.txt
  ```


### Jump to the Linux Instructions ###
1. Follow the rest of the instructions on how to use Rasware from the [clone rasware](#clone-rasware-1) step in the Linux instructions.


Setup for Windows (with VMware)
-------------------------------

### Install VMware ###
1. Download and install [VMware Workstation Player](https://my.vmware.com/web/vmware/downloads).
2. WMware Player is a hypervisor that lets us run a controlled environment for writing and flashing code.

### Download the RASBox Image ###
1. Download [RASBox](http://128.83.136.86/rasware/rasbox.ova).
2. Launch VMware Player.
3. Import the RASBox Image by selecting `Open a Virtual Machine` and navigating to the downloaded file.
4. If the import fails due to not passing `OVF specifications` or `Virtual hardware compliance checks`, just try again with checks relaxed.
5. Once imported, there should be a `rasbox` VM listed in VMware. Double click on it to launch the VM.
6. The user name is `ras` and there is no password.
7. First we need to set up the RASBox to use the available network interfaces. Find the available interfaces with the following command.

  ```bash
    ip link show
  ```

8. In the output there should be several interfaces with names like `eno16777736`. We should expect two different interfaces.
9. Run the following command with each interface to set up DHCP.

  ```bash
    sudo systemctl enable dhcpcd@<interface>
  ```

10. You should reboot to make sure the changes stick.

  ```bash
    sudo reboot now
  ```

11. You can check to make sure the network is working by pinging some website like google.

  ```bash
    ping www.gogle.com
  ```

### Forward USB ###
1. Plug in a Launchpad to your computer. If you don't have a Launchpad available this step can be done later.
2. In the RASBox VM, select `Player -> Removable Devices -> Luminary Micro ICDI` and select `Connect`.

### Install PuTTY (Optional but suggested) ###
1. Download [putty.exe](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html).
2. Putty is an incredibly useful little tool for terminal connections in Windows.
3. In VMWare, run the following command to find the ip address of the RASBox.

  ```bash
    ip addr show
  ```

4. Somewhere in the output there should be something like `inet 192.168.186.128`, this is your IP address. We most likely want the last one listed.
5. Launch Putty and set the hostname to `ras@<ip address>`.
6. You may want to also give the session a name and save it.
7. Click `Open` to create an SSH connection to the RASBox.

### Using the Terminal ###
1. You should now be presented with a terminal showing something like `ras@rasbox:~$ `. This is known as a [shell or command line](<https://en.wikipedia.org/wiki/Shell_(computing)>) which is the main interface for using our system.
2. Here is a quick introduction ([part 1](http://www.linfo.org/command_line_lesson_1.html), [part 2](http://www.linfo.org/command_line_lesson_2.html)) in using the shell.
3. To edit files, [nano](https://en.wikipedia.org/wiki/GNU_nano) is suggested for new users. You can edit a file, for example hello.txt below, by typing the following into the shell.

  ```bash
    nano hello.txt
  ```


### Jump to the Linux Instructions ###
1. Follow the rest of the instructions on how to use Rasware from the [clone rasware](#clone-rasware-1) step in the Linux instructions.


Setup for Mac
-------------

### Install Dependencies ###
1. Install Homebrew: [link](http://brew.sh) (Installation instructions are at the bottom of the page.)
2. Install OpenOCD through Homebrew in a terminal.

  ```bash
    brew install openocd
  ```

3. Download the Cross Compilers for the LM4F from [here](https://launchpad.net/gcc-arm-embedded) (download the one for Mac) and extract.
4. Download [StellarisWare](http://128.83.136.86/rasware/StellarisWare.tar.bz2), and extract.

### Clone Rasware ###

1. Use git to clone Rasware, replacing "username" with your Git username in the URL. Make sure you've forked Rasware already!

  ```bash
    git clone https://github.com/username/Rasware.git
  ```


### Modify the Makefiles to point to tools ###
1. In Rasware, open up RASLib/Makefile with your favorite text editor and change the line containing the `PREFIX` variable.
  ```make
    PREFIX := ../../gcc-arm-none-eabi-*/bin/arm-none-eabi-
  ```

2. Also change the line containing the `STELLARIS` and `CORTEXM4` variabls.
  ```make
    STELLARIS = ../../StellarisWare
    CORTEXM4 = ../../CortexM4Libs
  ```

3. Compile RASLib.

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

### Download StellarisWare ###
1. Download [StellarisWare](http://128.83.136.86/rasware/StellarisWare.tar.bz2), the TI library.
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

### Clone Rasware ###
1. Use git to clone Rasware, replacing "username" with your Git username in the URL. Make sure you've cloned Rasware already!

  ```bash
    git clone https://github.com/username/Rasware.git
  ```

2. Compile RASLib by using the [make](<https://en.wikipedia.org/wiki/Make_(software)>) program.

  ```bash
    cd Rasware/RASLib
    make
  ```


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

