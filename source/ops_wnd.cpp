/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

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



