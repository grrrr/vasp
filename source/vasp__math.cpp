/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vasp__op.h"

#include <math.h>


BL VecOp::d_opt(OpParam &p) 
{ 
	R v = 0;
	{
		const S *sr = p.rsdt;
		for(I i = 0; i < p.frames; ++i,sr += p.rss) { 
			register R s = fabs(*sr); 
			if(s > v) v = s; 
		}
	}

	if(v && v != 1) {
		v = 1./v;
		const S *sr = p.rsdt;
		S *dr = p.rddt;
		for(I i = 0; i < p.frames; ++i,dr += p.rds,sr += p.rss) 
			*dr = *sr * v;
	}
	return true; 
}

BL VecOp::d_copt(OpParam &p) 
{ 
	R v = 0;
	{
		const S *sr = p.rsdt,*si = p.isdt;
		for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss) { 
			register R s = sqabs(*sr,*si); 
			if(s > v) v = s; 
		}
	}

	if(v && v != 1) {
		v = 1./sqrt(v);
		const S *sr = p.rsdt,*si = p.isdt;
		S *dr = p.rddt,*di = p.iddt;
		for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,sr += p.rss,si += p.iss) {
			*dr = *sr * v, *di = *si * v;
		}
	}
	return true; 
}


Vasp *VaspOp::m_cpowi(Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	OpParam p("cpowi");
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		I powi = 1;
		if(arg.CanbeInt()) powi = arg.GetAInt();
		else
			post("%s - power arg is invalid -> set to 1",p.opname);

		if(powi < 0) {
			post("%s - negative integer power is not allowed",p.opname);
		}
		else {
			switch(powi) {
			case 0: {
				p.cbin.rarg = 1,p.cbin.iarg = 0;
				ret = DoOp(vecs,VecOp::d_cset,p);
				break;
			}
			case 1: {
				// set arg to src
				ret = DoOp(vecs,VecOp::d_ccopy,p);
				break;
			}
			case 2: {
				ret = DoOp(vecs,VecOp::d_csqr,p);
				break;
			}
			default: {
				p.cbin.rarg = powi,p.cbin.iarg = 0;
				ret = DoOp(vecs,VecOp::d_cpowi,p);
				break;
			}
			}
		}

		delete vecs;
	}
	return ret;
}



Vasp *VaspOp::m_rpow(Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	OpParam p("rpow");
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		CVecBlock *vecs = GetCVecs(p.opname,src,dst);
		if(vecs) {
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
			p.cbin.iarg = 0; // not used anyway

			ret = DoOp(vecs,VecOp::d_rpow,p);
			delete vecs;
		}
	}
	return ret;
}

