These are batch files and other accessories designed to aid in TASVideos publication work.

**Direct download link:**
  * [Encoding Tools](http://tastools.googlecode.com/files/EncodingUtilities.zip)

### Encoding ###

The scripts & files in this folder are for encoding TASVideos publications.  They are designed to go along with the [TASVideos encoding tutorial](http://tasvideos.org/EmulatorResources/MakingAVI/Common/Windows.html).

### MKV-split ###

This is a automated script designed to allow windows users to split MKV (and AVI) files into 1segments by dragging over one of the SplitMKV batch files.  This is intended to make segments of published movies in order to upload them to the [TASVideos Dailymotion group](http://www.dailymotion.com/TASVideos) and [TASVideos Youtube channel](http://www.youtube.com/OfficialTASVideos).

To use, drag the video file over the appropriate batch file (either dailymotion or youtube).  The script will split it into the needed segments.  That's all it takes!

The script is compatible with AVI files as well.  Simply drag them to the appropriate bat file as usual.  However it will give it a .mkv file extension.  It doesn't reencode teh file or anything, it is simply the changing of the extension.  It should work fine either way.  Or to avoid misleading extension they can be renamed back.

**Necessary downloads**
  * You will need mkvmerge.exe available [here](http://www.bunkus.org/videotools/mkvtoolnix/downloads.html#windows)
  * Download and install.
  * Browse to install location(default is C:\Program Files\MKVtoolnix), copy the mkvmerge.exe file to this directory.

Make sure you keep these files in the same directory as the batch files directory:
  * libiconv-2.dll
  * libintl-8.dll
  * magic1.dll
  * regex2.dll
  * SplitMKV.bat