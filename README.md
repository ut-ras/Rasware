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
2. Follow GitHub's great instructions for installing [Git](https://help.github.com/articles/set-up-git) on your computer.
3. If you want to learn more about Git, you can read GitPro online for free (and legally, too!) at http://git-scm.com/book

### Fork Rasware (only one Robotathon team member should do this step) ###

1. Once logged into GitHub, go to [ut-ras](https://github.com/ut-ras)'s [Rasware2013](https://github.com/ut-ras/Rasware2013).
2. [Fork](https://help.github.com/articles/fork-a-repo) the project's code through the button on the top right. This will copy the project to your own account.
3. On the team's forked repository Github page, go to Settings->Collaborators and add each team member as a collaborator

### Download the code

1. Each team member can now "clone" the team's forked repository to use on their local machines. In GitHub for Windows, simply click on `clone` next to the Rasware2013 project in the Github app. For linux, see the GitHub walkthough for using the command line to clone: https://help.github.com/articles/fork-a-repo.
2. In the Github app for Windows, right click on Rasware2013 then click on `open in explorer` to open the folder containing the project. 

Now that you and your team members have the code, let's look at how to compile it and run it on the Launchpad. 

Setup for Windows (WIP)
-----------------
### Install Stellarisware

1. [TODO: decide on correct link and finish this section]

### Install Keil uVision ###

1. Download the installer from here: [TODO: add correct download link]
2. When it's done, run the executable to install Keil uVision.

### Install PuTTY ###

1. Go to <http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html>.
2. Under "for Windows on Intel x86" click on [putty.exe](http://the.earth.li/~sgtatham/putty/latest/x86/putty.exe) to save putty to your computer.

### Install drivers for the Launchpad ###

Note: for windows 8, the drivers are unsigned, so installing them requires you to activate special permissions. Basically follow this process: http://www.tabletroms.com/forums/visual-land-tablets/5894-how-install-unsigned-drivers-windows-8-a.html. (Or else the Device Manager just won't let you install drivers.)

1. Go to <http://www.ti.com/tool/stellaris_icdi_drivers>.
2. Click `Download` to download the drivers to your computer.
3. Unzip to downloaded file.
4. Plug in a Stellaris Launchpad to your computer with a usb cord.
5. Windows should fail at installing divers.
6. Go to the Device Manager on your computer.
7. For each listing with a yellow warning, right-click and select `Update Driver`.
8. Click `Browse my Computer` and navigate to the unzipped drivers.
9. Once all of the drivers are installed, not the COM port number that appears in the Device manager. It should read `Stellaris Virtual Serial Port (COM__)`.

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

Setup for Linux (WIP)
---------------

TODO: Basically get Jimmy or Cruz to write this
