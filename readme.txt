VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)

----------------------------------------------------------------------------

Goals/features of the package:


----------------------------------------------------------------------------

Version history:

0.0.6:
- reorganized file structure
- changed name of vasp.n and vasp.n? to vasp.vector and vasp.vectors?, respectively
- added vasp.size, vasp.size+, vasp.size? for buffer size manipulation
- added vasp.peaks? for extremum extraction

0.0.5:
- fixed lacking sqrt in [vasp.rmin?],[vasp.rmax?]
- fixed (offs >= frames) bug in [vasp.offs?] 
- added lacking setup of [vasp.!-] and [vasp.c!-] objects 
- corrected buggy [vasp.int] code
- fixed recognition of integer arguments

0.0.4:
- changed vasp.min/max functions so that a vasp length 0 results in 0
- removed [vasp.inv], [vasp.cinv].... already replaced by [vasp.!/ 1]
- fixed outlet bug in [vasp.?]
- added right inlet to [vasp]... just like in [float] etc.

0.0.3:
- restructured the code for future use of break-point lists as arguments
- changed some object's names
- new objects: vasp.min?, vasp.max? and the likes
- fixed Max problem with connecting vasp.min,vasp.max right outlet to number boxes
- fixed right inlet problem for generator and filter objects

0.0.2:
- vasp.cmin,vasp.cmax - renamed to vasp.rmin,vasp.rmax
- reversed vasp channel and offset
- fixed bug with arguments to complex binary operations
- vasp.chk: included channel check
- fixed pointer bug in vasp.?
- fixed pointer increment bug in vasp.fhp
- changed VecBlock implementation

0.0.1:
- defined the vasp
- quick and dirty setup of most functions (non-interruptible)

---------------------------------------------------------------------------


TODO list:

features:
- introduce several log levels (for warning posts)
- flags how to handle special situations (div/0, log(<0) etc.)
- how handle symmetric data operations (x*): leave 0 and n-1 bin, odd remainder bin?
- progress % - outlet?
- filter object for radio commands
- asynchrone operations (multithreading, re-triggering) and interruptibility 
	- this implies a message queue at the inlet!
- grab (and recompose) signals for granular vasp usage
- vasp~s for realtime-capable vasp objects
- double precision variables?

- complex power operations - delta phase

- check and optimize fft routines
- optimize (c)osc with table lookup, phasor with Höldrichs code (in pd)

- clear samples outside transform domain (e.g. tilt)?

- how to handle beyond buffer-domain:
warning, treat as 0, calculate unit operation, loop, mirror on buffer end, stay on last frame

objects:
- wave object for table-lookup oscs.
- various random distributions for noise

- search functions for values -> offset

- envelopes
- gliss,slope -> stretch factor
- slope: frequence distortion with given exponent

bugs:


tests:
- vasp.size always keeps the data... do we want that? -> flag if yes/no


premature thoughts:
- interface to scripting language (python)
