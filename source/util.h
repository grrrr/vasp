/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_UTIL_H
#define __VASP_UTIL_H

#include "main.h"

#ifndef PI
#define PI 3.1415926535897932385
#endif

#define BIG 1.e10


R arg(R re,R im);
inline R arg(const CX &c) { return arg(c.real,c.imag); }
//inline F sqabs(const CX &c) { return sqabs(c.real,c.imag); }
//inline F sgn(F x) { return x < 0.?-1.F:1.F; }

template<class T>
inline T sqabs(T re,T im) { return re*re+im*im; }

template<class T>
inline V swap(T &a,T &b) { T c = a; a = b; b = c; }

template<class T>
inline T min(T a,T b) { return a < b?a:b; }

template<class T>
inline T max(T a,T b) { return a > b?a:b; }

#endif
