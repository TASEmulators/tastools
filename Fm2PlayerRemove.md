**Direct download link:**
  * [Fm2PlayerRemove](http://tastools.googlecode.com/files/Fm2-PlayerRemove.zip)

## SRC info ##

**author: adelikat** language: C++
**cross-platform**


## Introduction ##

This file is a utility specifically for [FM2](http://fceux.com/web/FM2.html) files.  These files are text based and if a user has gamepad 2 or fourscore enabled by accident, they can end up with 2-4 controllers worth of useless data.  This program is intended to strip away the unneeded controller info and fix the necessary header flags resulting in a much smaller and tidy input file.

It can receive a 2 controller or 4 controller movie and will stip it down to just 1 controller.

It must be fed the filename through commandline.  Windows users can simply drop the input file on the .exe.

It will output a fixed .fm2 file in the same directory as the existing.

Other combinations such as 3 controllers, or 4 controllers down to 2 can be made available upon request.