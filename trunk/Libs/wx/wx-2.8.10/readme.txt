I have left this stuff building mostly as it came from wx, as it had a pretty satisfying organization. Youll find the libs in lib/vc_lib

If someone wants to make a fully featured wx port, then they shouldn't be using this library.

Check include/wx/msw/setup.h for compile options.
(Do not be deceived by include/msvc/wx/setup.h, it is a betrayer.)

A lot of stuff was turned off, leaving only basic gui capabilities, in this build.
Look for comments //zero

This was done for desmume/win32 which only needed some basic wx dialogs.
Adding a feature or two should not be a problem, but you may have to go back to wx to grab the source in some cases (for example, wxjpeg)
