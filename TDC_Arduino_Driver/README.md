
README
======

This is the source code for the driver for the GP22 TDC chip eval board, programmed by Charles Baynham.

It is written in c++ and can be compiled and uploaded via the Arduino IDE. This code is intended to run on an Arduino Nano (or other Atmega328P board) to act as an interface to the GP22. The GP22 will be programmed with a default setup hardcoded here and then wait for commands. 

_IMPORTANT_
-----------

If you loaded this from the T: drive you may not have the latest version. This directory is a GIT repository and so contains a hidden folder called `.git` that has a record of every change made to this code since conception. In order to use this you must install a git client. https://windows.github.com/ is a simple one or http://www.sourcetreeapp.com/ is more full-featured. 

Once installed, "checkout" the master branch for the latest code version. 