/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

// --- osc ---------------------------------------

static BL d_osc(I cnt,F *dt,I str,Argument &a) 
{ 
	// frq and phase defined by complex frequency
	D perln = a.GetFloat(),cph = a.Next(1).GetFloat(), phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,cph += phinc,dt += str) *dt = cos(cph);
	return true;
}

static BL d_cosc(I cnt,F *re,I rstr,F *im,I istr,Argument &a) 
{ 
	// frq and phase defined by complex frequency
	D perln = a.GetFloat(),cph = a.Next(1).GetFloat(), phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,cph += phinc,re += rstr,im += istr) *re = cos(cph),*im = sin(cph);
	return true;
}

static BL d_mosc(I cnt,F *dt,I str,Argument &a) 
{ 
	// frq and phase defined by complex frequency
	D perln = a.GetFloat(),cph = a.Next(1).GetFloat(), phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,cph += phinc,dt += str) *dt *= cos(cph);
	return true;
}

static BL d_mcosc(I cnt,F *re,I rstr,F *im,I istr,Argument &a) 
{ 
	// frq and phase defined by complex frequency
	D perln = a.GetFloat(),cph = a.Next(1).GetFloat(), phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,cph += phinc,re += rstr,im += istr) {
		*re = cos(cph),*im = sin(cph);
	}
	return true;
}

Vasp *Vasp::m_osc(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0);
		return fr_prm("osc",d_osc,params); 
	}
	else return NULL;
}

Vasp *Vasp::m_cosc(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0);
		return fc_prm("cosc",d_cosc,params); 
	}
	else return NULL;
}

Vasp *Vasp::m_mosc(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0);
		return fr_prm("*osc",d_mosc,params); 
	}
	else return NULL;
}

Vasp *Vasp::m_mcosc(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0);
		return fc_prm("*cosc",d_mcosc,params);
	}
	else return NULL;
}


// --- phasor ---------------------------------------

// ! look up Höldrich's pd phasor code

static BL d_phasor(I cnt,F *dt,I str,Argument &a) 
{ 
	// frq and phase defined by complex frequency
	D perln = a.GetFloat(),cph = a.Next(1).GetFloat(), phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,cph += phinc,dt += str) *dt = fmod(cph,1.F);
	return true;
}

static BL d_mphasor(I cnt,F *dt,I str,Argument &a) 
{ 
	// frq and phase defined by complex frequency
	D perln = a.GetFloat(),cph = a.Next(1).GetFloat(), phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,cph += phinc,dt += str) *dt *= fmod(cph,1.F);
	return true;
}

Vasp *Vasp::m_phasor(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0);
		return fr_prm("phasor",d_phasor,params); 
	}
	else return NULL;
}

Vasp *Vasp::m_mphasor(const Argument &arg) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		Argument params;
		params.Set(flx::GetAFloat(arg.GetList()[0]));
		params.Add(arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0);
		return fr_prm("*phasor",d_mphasor,params); 
	}
	else return NULL;
}


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
	for(I i = cnt-1; i >= 0; --i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

static BL d_mbeveldn(I cnt,F *dt,I str,F) 
{ 
	for(I i = cnt-1; i >= 0; --i,dt += str) *dt *= (i+.5)/cnt;
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



