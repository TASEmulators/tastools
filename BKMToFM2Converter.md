## Download ##

[.BKM to .FM2 Converter r201](http://code.google.com/p/tastools/downloads/detail?name=.BKM%20to%20.FM2%20Converter%20r201.zip)

## Introduction ##

  * written by: Brandon Evans
  * language: C++

This program converts movies from .BKM (Also .TAS) to .FM2. It will covert the movie frames and all of the supported provided headers. Some equivalent headers will not be in a standard .BKM file (The ports used, NewPPU setting, etc.), but the most important ones are. It will also retrieve these headers if they were retained from a .FM2 to .BKM conversion, including the FCEUX version (Stored in the .BKM upon conversion as "emuOrigin"). FourScore compatibility was not investigated, and the resulting .FM2 file will always have the input for two controllers.

### Usage ###

To use it, you can invoke it from the command line with 0-2 parameters. If you provide no parameters, it will prompt you for the input and output file from standard input. If you provide 1, it will derive the output filename from the provided input filename. If you provide 2, it will use the first as the input filename and the second as the output filename.

### Supported Formats ###

  * FM2 version 3
  * BizHawk v0.0.1