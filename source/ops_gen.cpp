/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

// --- osc ---------------------------------------

static BL d_osc(I cnt,S *dt,I str,R perln,R ph) 
{ 
	R phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt = cos(ph);
	return true;
}

static BL d_cosc(I cnt,S *re,I rstr,S *im,I istr,R perln,R ph) 
{ 
	R phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,ph += phinc,re += rstr,im += istr) 
		*re = cos(ph),*im = sin(ph);
	return true;
}

static BL d_mosc(I cnt,S *dt,I str,R perln,R ph) 
{ 
	R phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt *= cos(ph);
	return true;
}

static BL d_mcosc(I cnt,S *re,I rstr,S *im,I istr,R perln,R ph) 
{ 
	R phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,ph += phinc,re += rstr,im += istr) {
		R zre = cos(ph),zim = sin(ph);

		register const R r = *re * zre - *im * zim;
		*im = *im * zre + *re * zim;
		*re = r;
	}
	return true;
}


/*! \brief Generator for real (cos) oscillations.

	\param arg argument list 
	\param arg.perlen Period length (in samples)
	\param arg.stph Starting phase
	\param mul true for multiplication to exisiting date
	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *Vasp::m_osc(const Argument &arg,BL mul) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		// period length
		R perlen = flx::GetAFloat(arg.GetList()[0]);
		// starting phase
		R stph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

		RVecBlock *vecs = GetRVecs(mul?"*osc":"osc",*this);
		if(vecs) {
			BL ok = true;
			for(I i = 0; ok && i < vecs->Vecs(); ++i) {
				VBuffer *s = vecs->Src(i);
				if(mul)
					ok = d_mosc(vecs->Frames(),s->Pointer(),s->Channels(),perlen,stph);
				else
					ok = d_osc(vecs->Frames(),s->Pointer(),s->Channels(),perlen,stph);
			}
			return ok?vecs->ResVasp():NULL;
		}
		else
			return NULL;
	}
	else return NULL;
}

/*! \brief Generator for complex (cos+i sin) oscillations.

	\param arg argument list 
	\param arg.perlen Period length (in samples)
	\param arg.stph Starting phase
	\param mul true for multiplication to exisiting date
	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *Vasp::m_cosc(const Argument &arg,BL mul) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		// period length
		R perlen = flx::GetAFloat(arg.GetList()[0]);
		// starting phase
		R stph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

		CVecBlock *vecs = GetCVecs(mul?"*cosc":"cosc",*this);
		if(vecs) {
			BL ok = true;
			for(I i = 0; ok && i < vecs->Pairs(); ++i) {
				VBuffer *re = vecs->ReSrc(i),*im = vecs->ImSrc(i);
				if(mul)
					ok = d_mcosc(vecs->Frames(),re->Pointer(),re->Channels(),im?im->Pointer():NULL,im?im->Channels():0,perlen,stph);
				else
					ok = d_cosc(vecs->Frames(),re->Pointer(),re->Channels(),im?im->Pointer():NULL,im?im->Channels():0,perlen,stph);
			}
			return ok?vecs->ResVasp():NULL;
		}
		else
			return NULL;
	}
	else return NULL;
}


// --- phasor ---------------------------------------

// ! look up Höldrich's pd phasor code

static BL d_phasor(I cnt,S *dt,I str,R perln,R ph) 
{ 
	// frq and phase defined by complex frequency
	R phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt = fmod(ph,1.F);
	return true;
}

static BL d_mphasor(I cnt,S *dt,I str,R perln,R ph) 
{ 
	// frq and phase defined by complex frequency
	R phinc = 2*PI/perln; 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt *= fmod(ph,1.F);
	return true;
}

/*! \brief Generator for sawtooth oscillations.

	\param arg argument list 
	\param arg.perlen Period length (in samples)
	\param arg.stph Starting phase
	\param mul true for multiplication to exisiting date
	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *Vasp::m_phasor(const Argument &arg,BL mul) 
{ 
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		// period length
		R perlen = flx::GetAFloat(arg.GetList()[0]);
		// starting phase
		R stph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

		RVecBlock *vecs = GetRVecs(mul?"*phasor":"phasor",*this);
		if(vecs) {
			BL ok = true;
			for(I i = 0; ok && i < vecs->Vecs(); ++i) {
				VBuffer *s = vecs->Src(i);
				if(mul)
					ok = d_mphasor(vecs->Frames(),s->Pointer(),s->Channels(),perlen,stph);
				else
					ok = d_phasor(vecs->Frames(),s->Pointer(),s->Channels(),perlen,stph);
			}
			return ok?vecs->ResVasp():NULL;
		}
		else
			return NULL;
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

static BL d_noise(I cnt,S *dt,I str) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt = rnd();
	return true;
}

static BL d_cnoise(I cnt,S *re,I rstr,S *im,I istr) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		S amp = rnd();
		S arg = rnd()*(2.*PI);
		*re = amp*cos(arg);
		*im = amp*sin(arg);
	}
	return true;
}


/*! \brief Generator for real valued noise.

	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *Vasp::m_noise() 
{ 
	RVecBlock *vecs = GetRVecs("noise",*this);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Vecs(); ++i) {
			VBuffer *s = vecs->Src(i);
			ok = d_noise(vecs->Frames(),s->Pointer(),s->Channels());
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}

/*! \brief Generator for complex noise (complex abs, complex arg).

	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *Vasp::m_cnoise() 
{ 
	CVecBlock *vecs = GetCVecs("cnoise",*this);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Pairs(); ++i) {
			VBuffer *re = vecs->ReSrc(i),*im = vecs->ImSrc(i);
			ok = d_cnoise(vecs->Frames(),re->Pointer(),re->Channels(),im->Pointer(),im->Channels());
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}


// --- bevel --------------------------

// Should bevels start from 0 or .5/cnt ??  -> 0!

static BL d_bevelup(I cnt,S *dt,I str) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

static BL d_mbevelup(I cnt,S *dt,I str) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt *= (i+.5)/cnt;
	return true;
}

static BL d_beveldn(I cnt,S *dt,I str) 
{ 
	for(I i = cnt-1; i >= 0; --i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

static BL d_mbeveldn(I cnt,S *dt,I str) 
{ 
	for(I i = cnt-1; i >= 0; --i,dt += str) *dt *= (i+.5)/cnt;
	return true;
}


/*! \brief Generator for bevel ups or downs.

	\param up true if bevel should rise
	\param mul true for multiplcation on existing data (aka fading)
	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *Vasp::m_bevelup(BL up,BL mul) 
{ 
	RVecBlock *vecs = GetRVecs(up?(mul?"*bevel":"bevel"):(mul?"*bevel-":"bevel-"),*this);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Vecs(); ++i) {
			VBuffer *s = vecs->Src(i);
			if(up)
				if(mul)
					ok = d_mbevelup(vecs->Frames(),s->Pointer(),s->Channels());
				else
					ok = d_bevelup(vecs->Frames(),s->Pointer(),s->Channels());
			else
				if(mul)
					ok = d_mbeveldn(vecs->Frames(),s->Pointer(),s->Channels());
				else
					ok = d_bevelup(vecs->Frames(),s->Pointer(),s->Channels());
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}

// --- window --------------------------

static BL d_window(I cnt,S *dt,I str,F wndtp) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

static BL d_vwindow(I cnt,S *dst,I dstr,const F *src,I sstr) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

static BL d_mwindow(I cnt,S *dt,I str,F wndtp) 
{ 
	post("vasp*window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt *= 1;
//	return true;
}

static BL d_vmwindow(I cnt,S *dst,I dstr,const F *src,I sstr) 
{ 
	post("vasp*window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt *= 1;
//	return true;
}


/*
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

*/

Vasp *Vasp::m_window(const Argument &arg,BL mul) 
{ 
	
	RVecBlock *vecs = GetRVecs(mul?"*window":"window",*this);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Vecs(); ++i) {
			VBuffer *s = vecs->Src(i);
			ok = d_noise(vecs->Frames(),s->Pointer(),s->Channels());
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}



