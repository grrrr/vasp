VASP modular - vector assembling signal processor
Object library for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

Donations for further development of the package are highly appreciated.

----------------------------------------------------------------------------

DOWNLOAD:
=========

http://www.parasitaere-kapazitaeten.net/vasp

----------------------------------------------------------------------------

GOALS/FEATURES:
===============

VASP is a package for PD or MaxMSP consisting of a number of externals extending 
these systems with functions for non-realtime array-based audio data processing. 
VASP is capable of working in the background, therefore not influencing eventual 
dsp signal processing.

----------------------------------------------------------------------------

COMPILATION:
============

For instruction on compilation of the package see the file compile.txt

You will need the flext C++ layer for PD and Max/MSP externals.
see http://www.parasitaere-kapazitaeten.net/ext/flext

----------------------------------------------------------------------------

IMPORTANT INFORMATION for all PD users:

For VASP and its documentation to work properly, you have to specify a 
PD command line like

pd -path vasp/pd -lib vasp/vasp

----------------------------------------------------------------------------

IMPORTANT INFORMATION for all MaxMSP users:

It is advisable to put the vasp object library file into the "max-startup" folder. 
Hence it will be loaded at Max startup.

If you want alternatively to load the vasp library on demand, 
create a "vasp" object somewhere. The library is then loaded.

----------------------------------------------------------------------------

