# TAS Button Stats #
  * written by: bkDJ
  * language: C++

## Download ##
[TAS Button Stats](http://code.google.com/p/tastools/downloads/detail?name=TAS_Button_Stats_v0.7.zip&can=2&q=)

## Introduction ##

TAS Button Stats is a program written to display the button stats of a movie file. Stats include how many times a button has been pressed, how many frames a button has been held for, etc. TODO: Expand.

Supported formats:
  * FCM
  * FM2
  * GMV (no support for XYZ buttons yet)
  * SMV
  * VBM
  * M64 (the analog stick is considered one button, either used or unused per frame)

## Related Lua Script: Button Count ##
  * written by: Brandon Evans
  * language: Lua

[Button Count](http://code.google.com/p/tastools/source/browse/trunk/Lua/ButtonCount.lua) is a script that tracks the buttons pressed and inputted in real time. In addition, if you save / load a state while running this script, these stats will go with them. Also, if you run the script while an supported movie is active (See below), it will parse the initial data out of it.

Supported formats:
  * FM2

Tested on:
  * FCEUX 2.1.4a, 2.1.5
  * Snes9x rerecording 1.51 v7 svn147
  * VBA-rr 22, 23.5a (Set controllers to 1 or it will count everything twice). Neither support stats saving / loading.

There's a [BizHawk equivalent](http://code.google.com/p/bizhawk/source/browse/trunk/BizHawk.MultiClient/output/Lua/ButtonCount.lua), bundled with the emulator, to this script. It works as well as the FCEUX version does with the additional benefits of being able to parse data out of the movie file even if you are re-recording and also doing so when you load a state that doesn't have data associated with it. This is the most accurate implementation we have to date.