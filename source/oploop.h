/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_OPLOOP_H
#define __VASP_OPLOOP_H

#define _D_BLOCK 1024

#define _D_MIN(a,b) ((a) < (b)?(a):(b))

#ifdef FLEXT_THREADS
#define _D_LOOP(VAR,LEN) \
	for(VAR = 0; VAR < LEN; flext_base::ThrYield()) \
	for(I __m__ = _D_MIN(LEN,i+_D_BLOCK); VAR < __m__; ++VAR)
#define _D_WHILE(COND) \
	for(; COND; flext_base::ThrYield()) \
	for(I __i__ = 0; __i__ < _D_BLOCK && (COND); ++__i__)
#else
#define _D_LOOP(VAR,LEN) \
	for(VAR = 0; VAR < LEN; ++VAR)
#define _D_WHILE(COND) \
	while(COND)
#endif

#endif
