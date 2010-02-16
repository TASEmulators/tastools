<?php
//TITLE=NES movie file format converter

$title = 'NES movie fileformat converter';
$progname = 'nesmock';

function usagetext($prog)
{
  exec('/usr/local/bin/'.$prog.' --help', $kk);
  $k='';foreach($kk as $s)$k.="$s\n";
  return $k;
}
 
$text = array(
   '1. Purpose' => "

<img src=\"http://bisqwit.iki.fi/src/nesmock-boxart.png\" align=\"left\" alt=\"nesmock\" />
This program converts different format NES movies to other formats.
 <p />
It supports movie files of the following emulators: Famtasia, FCE-Ultra (FCEU),
Nintendulator, VirtuaNES.
 <p />
This paragraph should be made much longer. It being too short now,
it looks somewhat stupid if the cool image on the left side is all alone
by itself. There should be more text in here. I'm not going to put some
latin text here, this rant must do. Something about documentation, or
about the freedom of open source.
 <p />
Nesmock does not support movie files for other systems such as SNES or Sega Genesis,
because those systems have different hardware.
<br clear=\"all\" />

", 'usage:1. Usage' => "

<pre class=smallerpre>".htmlspecialchars(usagetext('nesmock'))."</pre>

Offsets and delays are usually needed because different emulators
have different timings in various details.
Adjusting them is usually trial&amp;error work.
 <p>
If you are doubting whether this program actually works at all, try converting
Super Mario Bros movies. I have had complete success with them.
 <p>
Hint: Nesmock can be used for editing FCM files by first
converting them to FMV (which is easier to edit than FCM)
and then back to FCM.

", '1. Copying' => "

nesmock has been written by Joel Yliluoma, a.k.a.
<a href=\"http://iki.fi/bisqwit/\">Bisqwit</a>,<br>
and is distributed under the terms of the
<a href=\"http://www.gnu.org/licenses/licenses.html#GPL\">General Public License</a> (GPL).
 <p>
If you are an emulator developer, you are welcome to hack this source
code and add more format support to the code.<br>
These planned things are currently missing:
<ul>
 <li>Movie support for:
  <ul>
   <li>VirtuaNES (write)</li>
   <li>Nesticle (read, write)</li>
  </ul></li>
 <li>Savestate support for:
  <ul>
   <li>Nintendulator (read, write)</li>
   <li>FCEU (read, write (partial support exists))</li>
   <li>VirtuaNES (read (partial support exists), write)</li>
   <li>Famtasia (read, write)</li>
  </ul></li>
</ul>

", '1. Requirements' => "

GNU make and C++ compiler is required to recompile the source code.<br>
For the program (including the Windows binary), you need a commandline.<br>

", '1. Thoughs' => "

Different emulators emulate differently. This is a fact.
For this reason, usually straight conversions of the movies
don't just work.
 <p>
It is unfortunately impossible to guess from the movie file alone where
would the emulator need more / less frames than the other emulator.
 <p>
A more elaborate conversion could be made by running the two emulators
simultaneously and keeping track of their status, and when the other goes
desync, backtrack and bruteforce the combination that puts it back to track.
This kind of method is however extremely difficult to implement. And it
still might be impossible - if the game does calculations based on scanline
counters, there could be no way to make them match.

");
include '/WWW/progdesc.php';
