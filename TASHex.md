**Direct download links:**
  * [TASHex Windows](http://tastools.googlecode.com/files/TAS-Hex.zip)
  * [TASHex Linux](http://tastools.googlecode.com/files/tas-hexl.zip)

TAShex is a project started in 2007 with purpose of making movie file editing easy without having to calculate hex codes for button states and posiotions of needed segments. The idea was simple: program offers a way to convert movie file (as of release time, it supported every format, supported on site, which is not true for now) into few text files: one has header info, while other contain data for all controllers (one file per controller).

### Header file format: ###

<pre>
TEXT - file header<br>
TYPE=3 - type of original movie (not a subject to change)<br>
HEADER - here begins data from file header, in hex format, not subject to change)<br>
data<br>
POST - here begins data that goes after controller input data (might be save data for example)<br>
data<br>
CONTROLLERS<br>
<list of controller input data files><br>
</pre>

### Controller input file format ###

<pre>
CHARS=1234RLXA><v^SsYB - letters, corresponding to every bit of data.<br>
every next line is like that:<br>
<frame number>|<pressed buttons><br>
</pre>

frame number is not taken into account when encoding, it's there just for convenience of user to search for exact frame, so lines can be edited, moved, removed, copied and pasted without any worries.

### Supported formats: ###

vbm, smv, gmv, fcm, fmv - full

m64 - limited (because it's not frame-based, but input request-based)