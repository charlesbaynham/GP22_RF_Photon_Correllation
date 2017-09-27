NPL Backplane
=============

Usage
-----

_This is a Kicad Template: this whole folder should be copied into your "...\Documents\kicad\template\" folder. It can then be used be starting a new Kicad project with the "New Project from Template (Ctrl+T)" option._

Child board template - v1.0
---------------------------

This project template is designed for projects intending to use the NPL backplane system.

The NPL backplane supports 100x100mm boards and provides various supply voltages, as well as 30 GPIO ports and an interface for RS485 - serial communication. Protocol implementation is outside the scope of this document, but the Modbus protocol is recommended.

Power lines are not guaranteed to be quiet so noise-senstive applications should implement their own filtering or use an external supply. Maximum current draw is t.b.c. but other than the +15V supply line, other power supplies are expected to be relatively low current; intended for powering signal circuits only.

For more information, please contact Charles Baynham

(c) 2016 Charles Baynham