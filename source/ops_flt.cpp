/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


// --- highpass ---------------------------------------

static BL d_fhp(I cnt,F *dt,I str,Argument &a) 
{ 
	F perln = a.GetFloat();
	I tms = a.Next(1).GetFloat();

	F coef = 1-(2*PI)/perln;
    if(coef < 0) coef = 0;

	for(I ti = 0; ti < tms; ++ti) {
		register F v1;
		I i;

		// t+ direction
		for(i = 0,v1 = 0; i < cnt; ++i) {
			register const F v0 = *dt + coef*v1;
			*dt = v0-v1;
			v1 = v0;
			dt += str;
		}
		if(++ti == tms) break;

		// t- direction
		for(i = cnt-1,v1 = 0; i >= 0; --i) {
			dt -= str;
			register const F v0 = *dt + coef*v1;
			*dt = v0-v1;
			v1 = v0;
		}
	}

	return true;
}


Vasp *Vasp::m_fhp(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAInt(arg.GetList()[1]):1);
		return fr_prm("fhp",d_fhp,params); 
	}
	else return NULL;
}

// --- lowpass ---------------------------------------

static BL d_flp(I cnt,F *dt,I str,Argument &a) 
{ 
	F perln = a.GetFloat();
	I tms = a.Next(1).GetFloat();

	F coef = (2*PI)/perln;
    if(coef > 1) coef = 1;
	const F feed = 1-coef;

	for(I ti = 0; ti < tms; ++ti) {
		register F v1;
		I i;

		// t+ direction
		for(i = 0,v1 = 0; i < cnt; ++i) {
			v1 = *dt = coef* *dt + feed*v1;
			dt += str;
		}
		if(++ti == tms) break;

		// t- direction
		for(i = cnt-1,v1 = 0; i >= 0; --i) {
			dt -= str;
			v1 = *dt = coef* *dt + feed*v1;
		}
	}

	return true;
}


Vasp *Vasp::m_flp(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAInt(arg.GetList()[1]):1);
		return fr_prm("flp",d_flp,params); 
	}
	else return NULL;
}

