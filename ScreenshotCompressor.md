**Direct download link:**
  * [ScreenshotCompress](http://tastools.googlecode.com/files/ScreenshotCompress.zip)

## Introduction ##

This program is designed to use along with pngout.exe in order to compress [TASVideos](http://tasvideos.org) screenshot (.png files) as much as possible.

It is designed to compress a .png file optimally using pngout.exe.  (You will need this program for this to work).
It is intended to be used with 2D video game screenshots.
It runs 15 variations of the -b options in pngout and determines the lowest filesize.

ScreenshotCompress.exe, pngout.exe, and pngbat.bat must all be in the same directory.

### Using ###

There are several says to compress the file using this program.

1) Drag and drop

This is the main intention of this program.  Simply drag and drop your .png file onto ScreenshotCompress.exe and it will do the rest.
The file will be replaced with a compressed version.
The file can contain spaces, and does not need to be the same directory as ScreenshotCompress

2) Double click ScreenshotCompress.exe

If you double click the program, no filename is specified and it will default to test.png in the same folder.
If there is a test.png in the folder it will compress it, else it will error.

3) Specify a filename through commandline or a batch file

This will work the same as drag and drop.

### Source Code / Compiling ###

  * FileChooser.exe written in C++ (Written in Visual Studio 2008 but should compile with any compiler)
  * Console application