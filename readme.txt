VASP modular - vector assembling signal processor / objects for Max/MSP and PD
version 0.0.1

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

Donations for further development of the package are highly appreciated.

----------------------------------------------------------------------------

You will need the flext C++ layer for PD and Max/MSP externals to compile this.


Package files:
- readme.txt: this one
- gpl.txt,license.txt: GPL license stuff
- main.h,main.cpp: base class definition for all the other VASP objects

----------------------------------------------------------------------------

The package should at least compile (and is tested) with the following compilers:

- pd - Windows:
o Borland C++ 5.5 (free): makefile.bcc 
o Microsoft Visual C++ 6: vasp.dsp 

- pd - linux:
o GCC for linux: makefile.pd-linux 

- Max/MSP - MacOS:
o Metrowerks CodeWarrior V6: vasp project 

----------------------------------------------------------------------------

Goals/features of the package:


----------------------------------------------------------------------------

Version history:

0.0.1:
- defined the vasp
- quick and dirty setup of most functions (non-interruptible)

---------------------------------------------------------------------------


TODO list:

features:
- introduce several log levels (for warning posts)
- flags how to handle special situations (div/0, log(<0) etc.)
- flag for immediate or delayed explicit graphics refresh
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
- check for dst/src-overlaps in binary functions

- clear samples outside transform domain (e.g. tilt)?

- normalize -> optimize
- value outlet for optimize object

- how to handle beyond buffer-domain:
warning, treat as 0, calculate unit operation, loop, mirror on buffer end, stay on last frame

objects:
- buffer reference modification (offset, length)
- sync several parallel threads
- split und combine vasps into their separate vectors
- wave object for table-lookup oscs.
- various random distributions for noise

- spin: polar -> phase difference -> power -> cartesian   ( convention: abs(phase difference) <= pi ) 
- <,>
- div reverse

- search functions for values -> offset
- valleys

- envelopes
- gliss,slope -> stretch factor

- slope: frequence distortion with given exponent
- idist: complex integer power
- rgate: sample oriented noise gate

bugs:


tests:


premature thoughts:
- interface to scripting language (python)
