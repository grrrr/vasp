/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932385
#endif


// --- osc ---------------------------------------

static BL d_osc(I cnt,F *dt,I str,F *,I,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt = cos(ph);
	return true;
}

static BL d_cosc(I cnt,F *re,I rstr,F *im,I istr,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,re += rstr,im += istr) *re = cos(ph),*im = sin(ph);
	return true;
}

static BL d_mosc(I cnt,F *dt,I str,F *,I,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt *= cos(ph);
	return true;
}

static BL d_mcosc(I cnt,F *re,I rstr,F *im,I istr,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,re += rstr,im += istr) {
		*re = cos(ph),*im = sin(ph);
	}
	return true;
}

Vasp *Vasp::m_osc(const Argument &arg) 
{ return arg.CanbeComplex()?fc_arg("osc",arg.GetAComplex(),d_osc):NULL; }

Vasp *Vasp::m_cosc(const Argument &arg) 
{ return arg.CanbeComplex()?fc_arg("cosc",arg.GetAComplex(),d_cosc):NULL; }

Vasp *Vasp::m_mosc(const Argument &arg) 
{ return arg.CanbeComplex()?fc_arg("*osc",arg.GetAComplex(),d_mosc):NULL; }

Vasp *Vasp::m_mcosc(const Argument &arg) 
{ return arg.CanbeComplex()?fc_arg("*cosc",arg.GetAComplex(),d_mcosc):NULL; }


// --- phasor ---------------------------------------

// look up Höldrich's pd phasor code

static BL d_phasor(I cnt,F *dt,I str,F *,I,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt = fmod(ph,1.F);
	return true;
}

static BL d_mphasor(I cnt,F *dt,I str,F *,I,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt *= fmod(ph,1.F);
	return true;
}

Vasp *Vasp::m_phasor(const Argument &arg) 
{ return arg.CanbeComplex()?fc_arg("phasor",arg.GetAComplex(),d_phasor):NULL; }

Vasp *Vasp::m_mphasor(const Argument &arg) 
{ return arg.CanbeComplex()?fc_arg("*phasor",arg.GetAComplex(),d_mphasor):NULL; }


// --- noise --------------------------------

static F rnd() 
{
    static I val = 307*1319;
    F ret = ((F)((val&0x7fffffff)-0x40000000))*(F)(1.0/0x40000000);
    val = val * 435898247 + 382842987;
	return ret;
}

static BL d_noise(I cnt,F *dt,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt = rnd();
	return true;
}

static BL d_cnoise(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		F amp = rnd();
		F arg = rnd()*(2.*PI);
		*re = amp*cos(arg);
		*im = amp*sin(arg);
	}
	return true;
}

Vasp *Vasp::m_noise() { return fr_arg("noise",0,d_noise); }
Vasp *Vasp::m_cnoise() { return fc_arg("cnoise",0,d_cnoise); }

// --- bevel --------------------------

// Should bevels start from 0 or .5/cnt ??

static BL d_bevelup(I cnt,F *dt,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

static BL d_mbevelup(I cnt,F *dt,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt *= (i+.5)/cnt;
	return true;
}

static BL d_beveldn(I cnt,F *dt,I str,F) 
{ 
	for(I i = cnt-1; i >= 0; -i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

static BL d_mbeveldn(I cnt,F *dt,I str,F) 
{ 
	for(I i = cnt-1; i >= 0; -i,dt += str) *dt *= (i+.5)/cnt;
	return true;
}

Vasp *Vasp::m_bevelup() { return fr_arg("bevel",0,d_bevelup); }
Vasp *Vasp::m_beveldn() { return fr_arg("bevel-",0,d_beveldn); }
Vasp *Vasp::m_mbevelup() { return fr_arg("*bevel",0,d_mbevelup); }
Vasp *Vasp::m_mbeveldn() { return fr_arg("*bevel-",0,d_mbeveldn); }


// --- window --------------------------

static BL d_window(I cnt,F *dt,I str,F wndtp) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

static BL d_vwindow(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

static BL d_mwindow(I cnt,F *dt,I str,F wndtp) 
{ 
	post("vasp*window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt *= 1;
//	return true;
}

static BL d_vmwindow(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ 
	post("vasp*window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt *= 1;
//	return true;
}


Vasp *Vasp::m_window(const Argument &arg) 
{ 
	if(arg.CanbeInt())
		return fr_arg("window",arg.GetAInt(),d_window);
	else if(arg.IsVasp())
		return fv_arg("window",arg.GetVasp(),d_vwindow);
	else return NULL; 
}

Vasp *Vasp::m_mwindow(const Argument &arg) 
{ 
	if(arg.CanbeInt())
		return fr_arg("*window",arg.GetAInt(),d_mwindow);
	else if(arg.IsVasp())
		return fv_arg("*window",arg.GetVasp(),d_vmwindow);
	else return NULL; 
}



