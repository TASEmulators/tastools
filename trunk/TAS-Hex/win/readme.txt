TasHex v 0.0.2 by Lazy_Zefiris

supported formats:
vbm, smv, gmv, fcm, fmv - full
m64 - limited

//------------------------------------
usage:

tashex <script filename> 
  launches script (explained later)
tashex -t <filename> [<target filename>]
  translates movie to text file
tashex -c <filename> [<target filename>]
  translates text file to movie
tashex -d <filename> <x> <l>
  deletes <l> frames starting from frame number <x> (first frame has number 0)
tashex -r <destination> <x1> <x2> <source> <x3> <x4> [<target>]
  replaces frames x1-x2 in destination movie with frames x3-x4 in source file. saves to target, or destination file if target is not specified.

//------------------------------------
scripts:

comments begin with //
every line should contain 1 command

open <n> <fn>
  loads movie <fn> to slot <n> (n = 0..9)

savetext <n> <fn>
  saves movie from slot <n> to <fn> as text

savetext <n> <fn>
  saves movie from slot <n> to <fn>

delete <n> <x> <l>
  deletes <l> frames starting from <x> from movie in slot <n>

insert <n> <x> <l>
  inserts <l> zero frames starting from <x> into movie in slot <n>

set <n> <x> <l> <contr> <bit>
  sets <bit>th bit on <contr>th controller for <l> frames starting from <x> in movie in slot <n>

unset <n> <x> <l> <contr> <bit>
  unsets <bit>th bit on <contr>th controller for <l> frames starting from <x> in movie in slot <n>

copy <destin> <x1> <source> <x2> <l>
  inserts <l> frames from <source> slot movie stareing from <x2> into <destin> slot movie starting from <x1>

replace <dest> <x1> <x2> <source> <x1> <x2>
  replaces frames <x1>-<x2> in <dest> slot movie with frames <x1> <x2> in <source> slot

example script file:

open 1 test.fcm
open 2 test2.fcm
replace 1 45 70 2 45 55
save 1 test.fcm

equals to

tashex -r test.fcm 45 70 test2.fcm 45 55

I hope it's understandable and useful ^___^
