
README
======

This is the source code for the driver for the GP22 TDC chip eval board, programmed by Charles Baynham.

It is written in c++ and can be compiled and uploaded via the Arduino IDE. This code is intended to run on an Arduino Due (or other 3.3V Arduino board) to act as an interface to the GP22. When running, the Arduino will setup the GP22 with whatever configuration is stored in the GP22's EEPROM then wait for simple commands. 

N.B. The configuration of the GP22 is done via the software interface provided on http://ACAM.de. In particular, be cautious of the DELVAL1 setting in "Ultrasonic Time of Flight". This setting should be zero, but isn't by default. If not corrected, this will greatly limit the resolution of the device. 

_IMPORTANT_
-----------

If you loaded this from the T: drive you may not have the latest version. This directory is a GIT repository and so contains a hidden folder called `.git` that has a record of every change made to this code since conception. In order to use this you must install a git client. https://windows.github.com/ is a simple one or http://www.sourcetreeapp.com/ is more full-featured. 

Once installed, "checkout" the master branch for the latest code version. 