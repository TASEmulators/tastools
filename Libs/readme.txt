An increasing number of emulators in the rerecording community are using similar build paradigms:

1. VS-centered 
2. no external dependencies located outside the svn (simple: checkout, build, and go) 
3. maximum code reuse 
4. use of many 3rd-party libraries: zlib, lua, glib

Idea 2. helps bring the building of VS-centered projects to the masses, although this idea is not as rare as it once was.

A natural consequence of this is to want to checkin the source code for building these libraries into each emulator project. Surely this is better than just dropping in some libs built under unknown circumstances. This bloats the emulators considerably, and results in some duplication of labor due to slightly different overall build setups for the different emulators.

In an effort to cleanup this situation, the following proposal is made, to document and control the circumstances under which static libraries were built:

Let us make an archive of pre-built 3rd party libraries.  Each emulator should use these instead of its own custom-built libraries.

The libraries are named this way, for example:

zlib-vc8-Win32.lib 
zlib-vc8-x64.lib

Win32 is chosen because it is the default $(PlatformName) in visual studio.

The different build configurations are omitted to reduce clutter. Only the release builds are checked in. If someone needs to debug one of these libraries in-context in their emulator, then they need only go to tastools and build the debug versions which would be dropped into the emulator's directory structure temporarily.

The libraries are all built for now with vc8 with standard library symbols omitted, so that it can be linked in newer versions of visual studio. Libraries compiled with newer visual studios often have problems linking to emulators being compiled in older ones. Of course, this may result in suboptimal performance in some cases. This may be addressed as the need arises.

These libraries are all, for now, built with the multithreaded non-dll CRT libraries. Some emulators may have different requirements...

Various other build properties, such as link-time code generation, whole program optimization, and other optimization properties, may vary from library to library according to the library builder's whim, as he attempts to balance performance against humongousness of libraries.

The emulator should drop the libraries and corresponding header files into its own directory tree, as well as a memo indicating the source of these libraries. For example, create a file with the name:

zlib-1.2.3_from_tastools

and no contents.

Also conspicuously absent from the library name is the version number of the library. This information would be conveyed by the memo file anyway.

As a consequence of these naming conventions, libraries can be upgraded without having to fiddle with emulator link inputs or link directories, and it is easy to use them since there are not a dozen different library names for different build configurations.

These approaches have been verified with vs2005 and vs2008; vs2010 is an unusable heap of trouble and we haven't yet verified how well these libs will work in it.