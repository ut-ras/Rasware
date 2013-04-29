Rasware2013
===========

Description
-----------

A generic robotics controller for the EK-LM4F120XL Stellaris Launchpad. 
Authored by the IEEE - Robotics and Automation Society Student Branch at the University of Texas at Austin.


Setup for Windows (WIP)
-----------------

### Getting started with Git ###

1. Create a free GitHub account on [github.com](https://github.com/signup/free)
2. Follow GitHub's great instructions for installing [Git](https://help.github.com/articles/set-up-git) on your computer.

### Forking Rasware ###

1. Once logged into GitHub, go to [ut-ras](https://github.com/ut-ras)'s [Rasware2013](https://github.com/ut-ras/Rasware2013).
2. [Fork](https://help.github.com/articales/fork-a-repo) the project's code through the button on the top right. This will copy the project to your own account.
3. Open Git. Depending on what version is installed, the commands will be slightly different.
4. Clone the project to create a copy on your computer. In GitHub for Windows, simply click on `clone` next to the Rasware2013 project.
5. Right click on Rasware2013 then click on `open in explorer` to open the folder containing the project.

### Installing Putty ###

1. Go to <http://www.chiark.greenend.org.uk/~sgtatham/putty.download.html>.
2. Under "for Windows on Intel x86" click on [putty.exe](http://the.earth.li/~sgtatham/putty/latest/x86/putty.exe) to save putty to your computer.

### Installing Keil uVision ###

<!--1. Go to <http://www.keil.com/demo/eval/arm.htm>.
2. To download Keil you will need to enter contact information.
3. Click `MDK470A.EXE` to download the installer.
4. Run the executable to install Keil uVision.

Actually this link may just work for a while -->

1. Download the installer from here: <http://www.keil.com/fid/w00kzbwyie9j1wbbrv11bm0or1xbb2p1gd2wd1/files/eval/mdk470a.exe>
2. When it's done, run the executable to install Keil uVision.


### Installing the Drivers ###

1. Go to <http://www.ti.com/tool/stellaris_icdi_drivers>.
2. Click `Download` to download the drivers to your computer.
3. Unzip to downloaded file.
4. Plug in a Stellaris Launchpad to your computer with a usb cord.
5. Windows should fail at installing divers.
6. Go to the Device Manager on your computer.
7. For each listing with a yellow warning, right-click and select `Update Driver`.
8. Click `Browse my Computer` and navigate to the unzipped drivers.
9. Once all of the drivers are installed, not the COM port number that appears in the Device manager. It should read `Stellaris Virtual Serial Port (COM29)`.


TODO: add the sections for running rasdemo and creating team project.
TODO: add additional information section from Rasware2012

Setup for Linux (WIP)
---------------

TODO: Basically get Jimmy to write this
