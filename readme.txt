VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)

----------------------------------------------------------------------------

Goals/features of the package:


----------------------------------------------------------------------------

Version history:

0.0.7:
- CHANGE: vasp.m is now called vasp.multi by default
- CHANGE: vasp.sync and vasp.multi now have a default argument = 2
- FIX: vasp.phasor - period length now 1 instead of 2 PI
- FIX: bug in multi mode (more src, one arg vector)
- ADD: path specification for help symbols
- CHANGE: rewrote vasp.imm implementation (still very inefficient!!)
- ADD: prior message defines differential priority for detached operations
- ADD: full DFT implementation (radix-2/radix-n) - also support of "to" message
- ADD: full implementation of tilt/xtilt with several interpolation methods
- CHANGE: vasp.split and vasp.join now have default argument = 2

0.0.6:
- reorganized file structure
- ADD: main names of vasp.n and vasp.n? are now vasp.vector and vasp.vectors?, respectively
- ADD: main names of vasp.? and vasp.?? are now vasp.list and vasp.nonzero, respectively
- NEW: vasp.size, vasp.size+, vasp.size? (vasp.s,vasp.s+,vasp.s?) for buffer size manipulation
- NEW: vasp.peaks? for extremum extraction
- ADD: shortcuts for vasp.frames,vasp.frames+,vasp.frames? -> vasp.f,vasp.f+,vasp.f?
- CHANGE: changed vasp.offs,vasp.offs+,vasp.offs? to vasp.offset,vasp.offset+,vasp.offset?
- ADD: shortcuts for vasp.offset,vasp.offset+,vasp.offset? -> vasp.o,vasp.o+,vasp.o?
- ADD: envelopes (env ...) as arguments to all operations where vasps are used
- FIX: default arguments for all binary/anytype operations
- ADD: vasp.frames* (vasp.f*), vasp.frames/ (vasp.f/), vasp.size* (vasp.s*), vasp.size/ (vasp.s/)
- ADD: detached operation: operations run as threads, according to detach flag/message
- CHANGE: vasp.sync has as many outputs as inputs and outputs all input vasps
- ADD: vasp.! : like vasp but stores the content temporarily (not just the reference)
- ADD: vasp.copy (vasp.->) and vasp.ccopy (vasp.c->) for instant vasp copying
- ADD: vasp.radio and vasp.noradio (vasp.!radio) ... filters for radio messages
- ADD: vasp.fix - bashes NANs to zero, normalizes denormal numbers
- ADD: double type consisting of 2 additive floats (e.g. "double 1. 1.e-13") for all numeric arguments
- ADD: vasp.(x)shift - "fill" method/flag defines how to fill shifted areas (0..zero (default),1..none,2..edge value)

0.0.5:
- FIX: lacking sqrt in [vasp.rmin?],[vasp.rmax?]
- FIX: (offs >= frames) bug in [vasp.offs?] 
- ADD: lacking setup of [vasp.!-] and [vasp.c!-] objects 
- FIX: buggy [vasp.int] code
- FIX: recognition of integer arguments

0.0.4:
- CHANGE: vasp.min/max functions so that a vasp length 0 results in 0
- REMOVED: [vasp.inv], [vasp.cinv].... already replaced by [vasp.!/ 1]
- FIX: outlet bug in [vasp.?]
- ADD: right inlet to [vasp]... just like in [float] etc.

0.0.3:
- restructured the code for future use of break-point lists (aka envelopes) as arguments
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
---------------------
- introduce several log levels (for warning posts)
- how handle symmetric data operations (x*): leave 0 and n-1 bin, odd remainder bin?
- progress % - outlet?
- grab (and recompose) signals for granular vasp usage
- vasp~s for realtime-capable vasp objects

- complex power operations - delta phase

- optimize (c)osc with table lookup, phasor with Höldrichs code (in pd)

- how to handle beyond buffer-domain:
warning, treat as 0, calculate unit operation, loop, mirror on buffer end, stay on last frame

- set thread priority (introduce feature into flext!)
- better algorithm for vasp.fix

- make loops granular and interruptible
- make in place rotation more efficient (use temporary space)

- introduce default values (2) for vasp.split and vasp.join

- vasp.clip for 0..1 clipping

objects:
---------------------
- wave object for table-lookup oscs.
- various random distributions for noise

- search functions for values -> offset

- slope: frequency distortion with given exponent

- vasp.trigger object
- vasp.expr


bugs:
---------------------


tests:
---------------------
- vasp.size always keeps the data... do we want that? -> flag if yes/no
- in-place operation (src == dst), test if strides are equal!


premature thoughts:
---------------------
- interface to scripting language (python)



documentation:
---------------------
- have all the objects written in their full names
- no special characters!




4 fundamental parameter types:
real - float 
complex - list with 2 floats
env - list with >= 3 float values
vasp - list with ([f] s [f] [f] ) structure

