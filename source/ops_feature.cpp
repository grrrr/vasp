/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>


// --- integrate/differentiate

static BL d_int(I cnt,S *dst,I str,S) 
{ 
	if(cnt >= 2) {
		register F d = *dst; 
		for(I i = 1; i < cnt; ++i) { register S d1 = *(dst += str); *dst = d1+d,d = d1; }
	}
	return true; 
}

static BL d_dif(I cnt,S *dst,I str,S) 
{ 
	if(cnt >= 2) {
		register S d = *dst; *dst = 0;
		for(I i = 1; i < cnt; ++i) { register S d1 = *(dst += str); *dst = d1-d,d = d1; }
	}
	return true; 
}

Vasp *Vasp::m_int() { return fr_arg("int",0,d_int); }
Vasp *Vasp::m_dif() { return fr_arg("dif",0,d_dif); }


// --- find peaks

// complex peaks (radius)

// how to treat <=,>= ?

static BL d_peaks(I cnt,S *_dst,I str,F tms) 
{ 
	if(cnt >= 2) {
		I itms = (I)tms;
		while(itms--) {
			F *dst = _dst;
			register F *dd = dst,d = fabs(*dd),dn = fabs(*(dst += str));
			for(I i = 2; i < cnt; ++i) { 
				if(d < dn) *dd = 0;
				
				for(dd = dst,d = dn; (dn = fabs(*(dst += str))) == 0 && i < cnt; ++i);
			}
			if(dn > d) *dst = 0;
		}
	}
	return true; 
}

Vasp *Vasp::m_peaks(const Argument &arg) 
{ return arg.CanbeInt()?fr_arg("peaks",arg.GetAInt(),d_peaks):NULL; }
