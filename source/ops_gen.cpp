/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

// --- osc ---------------------------------------

/*!
	\brief Generator for real (cos) oscillations
	\todo check for reverse operation!
*/
BL VecOp::d_osc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc; 
	for(; p.frames--; ph += phinc,p.rddt += p.rds) *p.rddt = cos(ph);
	p.gen.ph = ph;
	return true;
}

/*!
	\brief multiplicative generator for real (cos) oscillations
	\todo check for reverse operation!
*/
BL VecOp::d_mosc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc; 
	for(; p.frames--; ph += phinc,p.rsdt += p.rss,p.rddt += p.rds) *p.rddt = *p.rsdt * cos(ph);
	p.gen.ph = ph;
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
Vasp *VaspOp::m_osc(Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	OpParam p(mul?"*osc":"osc");
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			// period length
			p.gen.phinc = 2*PI/flx::GetAFloat(arg.GetList()[0]); 
			// starting phase
			p.gen.ph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

/*
			VecOp::opfun *fun;
			if(mul) {
				if(p.SROvr()) p.SDRRev();
				fun = VecOp::d_mosc;
			}
			else fun = VecOp::d_osc;
*/		
			ret = DoOp(vecs,mul?VecOp::d_mosc:VecOp::d_osc,p);
			delete vecs;
		}
	}

	return ret;
}


/*!
	\brief Generator for complex oscillations.
	\todo check for reverse operation!
*/
BL VecOp::d_cosc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	for(; p.frames--; ph += phinc,p.rddt += p.rds,p.iddt += p.ids) 
		*p.rddt = cos(ph),*p.iddt = sin(ph);
	p.gen.ph = ph;
	return true;
}

/*!
	\brief Multiplicative generator for complex oscillations.
	\todo check for reverse operation!
*/
BL VecOp::d_mcosc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	for(; p.frames--; ph += phinc,p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids) {
		R zre = cos(ph),zim = sin(ph);

		register const R r = *p.rsdt * zre - *p.isdt * zim;
		*p.iddt = *p.isdt * zre + *p.rsdt * zim;
		*p.rddt = r;
	}
	p.gen.ph = ph;
	return true;
}

/*! \brief Generator for complex (cos+i sin) oscillations.

	\param arg argument list 
	\param arg.perlen Period length (in samples)
	\param arg.stph Starting phase
	\param mul true for multiplication to exisiting date
	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *VaspOp::m_cosc(Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	OpParam p(mul?"*cosc":"cosc");
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		CVecBlock *vecs = GetCVecs(p.opname,src,dst);
		if(vecs) {
			// period length
			p.gen.phinc = 2*PI/flx::GetAFloat(arg.GetList()[0]); 
			// starting phase
			p.gen.ph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

/*
			VecOp::opfun *fun;
			if(mul) {
				BL rovr = p.SROvr(),iovr = p.SIOvr(); 
				if(rovr == iovr) { p.SDRRev(); p.SDIRev(); }
				else {
					post("%s: Unresolvable src/dst vector overlap",opnm);
					return NULL;
				}
				fun = VecOp::d_mcosc;
			}
			else fun = VecOp::d_cosc;
*/
		
			ret = DoOp(vecs,mul?VecOp::d_mcosc:VecOp::d_cosc,p);
			delete vecs;
		}
	}

	return ret;
}


// --- phasor ---------------------------------------

// ! look up Höldrich's pd phasor code

/*!
	\brief Generator for saw wave oscillations.
	\todo check for reverse operation!
*/
BL VecOp::d_phasor(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	for(; p.frames--; ph += phinc,p.rddt += p.rds) *p.rddt = fmod(ph,1.F);
	p.gen.ph = ph;
	return true;
}

/*!
	\brief Multiplicative generator for saw wave oscillations.
	\todo check for reverse operation!
*/
BL VecOp::d_mphasor(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	for(; p.frames--; ph += phinc,p.rddt += p.rds,p.rsdt += p.rss) *p.rddt = *p.rsdt * fmod(ph,1.F);
	p.gen.ph = ph;
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
Vasp *VaspOp::m_phasor(Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	OpParam p(mul?"*phasor":"phasor");
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			// period length
			p.gen.phinc = 2*PI/flx::GetAFloat(arg.GetList()[0]); 
			// starting phase
			p.gen.ph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;
		
/*
			VecOp::opfun *fun;
			if(mul) {
				if(p.SROvr()) p.SDRRev();
				fun = VecOp::d_mphasor;
			}
			else fun = VecOp::d_phasor;
*/		
			ret = DoOp(vecs,mul?VecOp::d_mphasor:VecOp::d_phasor,p);
			delete vecs;
		}
	}

	return ret;
}


// --- noise --------------------------------

static F rnd() 
{
    static I val = 307*1319;
    F ret = ((F)((val&0x7fffffff)-0x40000000))*(F)(1.0/0x40000000);
    val = val * 435898247 + 382842987;
	return ret;
}

BL VecOp::d_noise(OpParam &p) 
{ 
	for(; p.frames--; p.rddt += p.rds) *p.rddt = rnd();
	return true;
}


/*! \brief Generator for real valued noise.

	\return normalized destination vasp
*/
Vasp *VaspOp::m_noise(Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	OpParam p("noise");
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_noise,p);
		delete vecs;
	}
	return ret;
}

BL VecOp::d_cnoise(OpParam &p) 
{ 
	for(; p.frames--; p.rddt += p.rds,p.iddt += p.ids) {
		R amp = rnd();
		R arg = rnd()*(2.*PI);
		*p.rddt = amp*cos(arg);
		*p.iddt = amp*sin(arg);
	}
	return true;
}

/*! \brief Generator for complex noise (complex abs, complex arg).

	\return normalized destination vasp

	\todo Replace period length by frequency specification
*/
Vasp *VaspOp::m_cnoise(Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	OpParam p("cnoise");
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_cnoise,p);
		delete vecs;
	}
	return ret;
}


// --- bevel --------------------------

// Should bevels start from 0 or .5/cnt ??  -> 0!

//! \todo check for reverse operation!
BL VecOp::d_bevel(OpParam &p) 
{ 
	register R cur = p.bvl.cur,inc = p.bvl.inc;
	for(; p.frames--; p.rddt += p.rds,cur += inc) *p.rddt = cur;
	p.bvl.cur = cur;
	return true;
}

//! \todo check for reverse operation!
BL VecOp::d_mbevel(OpParam &p) 
{ 
	register R cur = p.bvl.cur,inc = p.bvl.inc;
	for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds,cur += inc) *p.rddt = *p.rsdt * cur;
	p.bvl.cur = cur;
	return true;
}

/*! \brief Generator for bevel ups or downs.

	\param up true if bevel should rise
	\param mul true for multiplcation on existing data (aka fading)
	\return normalized destination vasp
*/
Vasp *VaspOp::m_bevelup(Vasp &src,Vasp *dst,BL up,BL mul) 
{ 
	Vasp *ret = NULL;
	OpParam p(up?(mul?"*bevel":"bevel"):(mul?"*bevel-":"bevel-"));
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
	
		VecOp::opfun *fun;
		if(mul) {
			p.bvl.cur = up?0:1; // start
			p.bvl.inc = (up?1.:-1.)/vecs->Frames(); // increase

//			if(p.SROvr()) p.SDRRev();
			fun = VecOp::d_mbevel;
		}
		else {
			p.bvl.cur = up?1:0; // start
			p.bvl.inc = (up?-1.:1.)/vecs->Frames(); // increase

			fun = VecOp::d_bevel;
		}
	
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}


#if 0

// --- window --------------------------

BL VecOp::d_window(I cnt,S *dt,I str,I wndtp) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

BL VecOp::d_vwindow(I cnt,S *dst,I dstr,const S *src,I sstr) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

BL VecOp::d_mwindow(I cnt,S *dt,I str,I wndtp) 
{ 
	post("vasp*window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt *= 1;
//	return true;
}

BL VecOp::d_vmwindow(I cnt,S *dst,I dstr,const S *src,I sstr) 
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

Vasp *VaspOp::m_window(Vasp &dst,const Argument &arg,BL mul) 
{ 
	
	RVecBlock *vecs = GetRVecs(mul?"*window":"window",dst);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Vecs(); ++i) {
			VBuffer *s = vecs->Src(i);
			ok = VecOp::d_noise(vecs->Frames(),s->Pointer(),s->Channels());
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}

#endif

