/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

// --- osc ---------------------------------------

/*!	\brief Generator for real (cos) oscillations
*/
BL VecOp::d_osc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc; 
	if(p.revdir) ph -= (p.frames-1)*(phinc = -phinc);

	for(I i = 0; i < p.frames; ++i,ph += phinc,p.rddt += p.rds) *p.rddt = cos(ph);
	return true;
}

/*!	\brief multiplicative generator for real (cos) oscillations
*/
BL VecOp::d_mosc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc; 
	if(p.revdir) ph -= (p.frames-1)*(phinc = -phinc);

	for(I i = 0; i < p.frames; ++i,ph += phinc,p.rsdt += p.rss,p.rddt += p.rds) 
		*p.rddt = *p.rsdt * cos(ph);
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
Vasp *VaspOp::m_osc(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			// period length
			p.gen.phinc = 2*PI/flx::GetAFloat(arg.GetList()[0]); 
			// starting phase
			p.gen.ph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

			ret = DoOp(vecs,mul?VecOp::d_mosc:VecOp::d_osc,p);
			delete vecs;
		}
	}

	return ret;
}


/*!	\brief Generator for complex oscillations.
*/
BL VecOp::d_cosc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	if(p.revdir) ph -= (p.frames-1)*(phinc = -phinc);

	for(; p.frames--; ph += phinc,p.rddt += p.rds,p.iddt += p.ids) 
		*p.rddt = cos(ph),*p.iddt = sin(ph);
	return true;
}

/*!	\brief Multiplicative generator for complex oscillations.
*/
BL VecOp::d_mcosc(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	if(p.revdir) ph -= (p.frames-1)*(phinc = -phinc);

	for(; p.frames--; ph += phinc,p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids) {
		R zre = cos(ph),zim = sin(ph);

		register const R r = *p.rsdt * zre - *p.isdt * zim;
		*p.iddt = *p.isdt * zre + *p.rsdt * zim;
		*p.rddt = r;
	}
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
Vasp *VaspOp::m_cosc(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		CVecBlock *vecs = GetCVecs(p.opname,src,dst);
		if(vecs) {
			// period length
			p.gen.phinc = 2*PI/flx::GetAFloat(arg.GetList()[0]); 
			// starting phase
			p.gen.ph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;

			ret = DoOp(vecs,mul?VecOp::d_mcosc:VecOp::d_cosc,p);
			delete vecs;
		}
	}

	return ret;
}


// --- phasor ---------------------------------------

// ! look up Höldrich's pd phasor code

/*!	\brief Generator for saw wave oscillations.
*/
BL VecOp::d_phasor(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	if(p.revdir) ph -= (p.frames-1)*(phinc = -phinc);

	for(; p.frames--; ph += phinc,p.rddt += p.rds) *p.rddt = fmod(ph,1.F);
	return true;
}

/*!	\brief Multiplicative generator for saw wave oscillations.
*/
BL VecOp::d_mphasor(OpParam &p) 
{ 
	register R ph = p.gen.ph,phinc = p.gen.phinc;
	if(p.revdir) ph -= (p.frames-1)*(phinc = -phinc);

	for(; p.frames--; ph += phinc,p.rddt += p.rds,p.rsdt += p.rss) *p.rddt = *p.rsdt * fmod(ph,1.F);
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
Vasp *VaspOp::m_phasor(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			// period length
			p.gen.phinc = 2*PI/flx::GetAFloat(arg.GetList()[0]); 
			// starting phase
			p.gen.ph = arg.GetList().Count() >= 2?flx::GetAFloat(arg.GetList()[1]):0;
		
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

/*!	\brief Vector function for pseudorandom noise.
*/
BL VecOp::d_noise(OpParam &p) 
{ 
	for(; p.frames--; p.rddt += p.rds) *p.rddt = rnd();
	return true;
}


/*! \brief Generator for real valued noise.

	\return normalized destination vasp
*/
Vasp *VaspOp::m_noise(OpParam &p,Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_noise,p);
		delete vecs;
	}
	return ret;
}

/*!	\brief Vector function for pseudorandom complex noise.
*/
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
Vasp *VaspOp::m_cnoise(OpParam &p,Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_cnoise,p);
		delete vecs;
	}
	return ret;
}


// --- bevel --------------------------

// Should bevels start from 0 or .5/cnt ??  -> 0!

/*!	\brief Vector function for bevel.
*/
BL VecOp::d_bevel(OpParam &p) 
{ 
	register R cur = p.bvl.cur,inc = p.bvl.inc;
	if(p.revdir) cur -= (p.frames-1)*(inc = -inc);

	register S *dd = p.rddt;
	if(p.rds == 1)
		for(I i = 0; i < p.frames; ++i,dd++,cur += inc) *dd = cur;
	else
		for(I i = 0; i < p.frames; ++i,dd += p.rds,cur += inc) *dd = cur;
	return true;
}

/*!	\brief Vector function for multiplicative bevel ("fade").
*/
BL VecOp::d_mbevel(OpParam &p) 
{ 
	register R cur = p.bvl.cur,inc = p.bvl.inc;
	if(p.revdir) cur -= (p.frames-1)*(inc = -inc);

	register const S *sd = p.rsdt;
	register S *dd = p.rddt;
	if(sd == dd)
		if(p.rss == 1 && p.rds == 1)
			for(I i = 0; i < p.frames; ++i,dd++,cur += inc) *dd *= cur;
		else
			for(I i = 0; i < p.frames; ++i,dd += p.rds,cur += inc) *dd *= cur;
	else
		if(p.rss == 1 && p.rds == 1)
			for(I i = 0; i < p.frames; ++i,sd++,dd++,cur += inc) *dd = *sd * cur;
		else
			for(I i = 0; i < p.frames; ++i,sd += p.rss,dd += p.rds,cur += inc) *dd = *sd * cur;
	return true;
}

/*! \brief Generator for bevel ups or downs.

	\param up true if bevel should rise
	\param mul true for multiplcation on existing data (aka fading)
	\return normalized destination vasp
*/
Vasp *VaspOp::m_bevelup(OpParam &p,Vasp &src,Vasp *dst,BL up,BL mul) 
{ 
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		p.bvl.cur = up?0:1; // start
		p.bvl.inc = (up?1.:-1.)/vecs->Frames(); // increase

		ret = DoOp(vecs,mul?VecOp::d_mbevel:VecOp::d_bevel,p);
		delete vecs;
	}

	return ret;
}


// --- window --------------------------

BL VecOp::d_window(OpParam &p) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

BL VecOp::d_vwindow(OpParam &p) 
{ 
	post("vasp.window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt = 0;
//	return true;
}

BL VecOp::d_mwindow(OpParam &p) 
{ 
	post("vasp*window: Sorry, not implemented yet");
	return false;
	
//	for(I i = 0; i < cnt; ++i,dt += str) *dt *= 1;
//	return true;
}

BL VecOp::d_vmwindow(OpParam &p) 
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

Vasp *VaspOp::m_window(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst,BL mul) 
{ 
	Vasp *ret = NULL;
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		RVecBlock *vecs = GetRVecs(p.opname,src,dst);
		if(vecs) {
			// window mode
			p.wnd.wndtp = flx::GetAInt(arg.GetList()[1]);
		
			ret = DoOp(vecs,mul?VecOp::d_mwindow:VecOp::d_window,p);
			delete vecs;
		}
	}

	return ret;
}



