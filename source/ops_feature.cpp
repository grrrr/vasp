/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>


// --- integrate/differentiate

/*! \brief Integration
	\remark The delay of the result is +/- one sample, depending on the direction of the calculation 
	
	\todo different modes how to initialize first carry?
	\todo repetition count
*/
BL VecOp::d_int(OpParam &p) 
{ 
	register S d = p.intdif.carry;
	for(; p.frames-- ; p.rsdt += p.rss,p.rddt += p.rds) { 
		register S d1 = *p.rsdt; 
		*p.rddt = d1+d,d = d1; 
	}
	p.intdif.carry = d;
	return true; 
}

/*! \brief Differentiation
	\remark The delay of the result is +/- one sample, depending on the direction of the calculation 

	\todo different modes how to initialize first carry?
	\todo repetition count
*/
BL VecOp::d_dif(OpParam &p) 
{ 
	register S d = p.intdif.carry;
	for(; p.frames-- ; p.rsdt += p.rss,p.rddt += p.rds) { 
		register S d1 = *p.rsdt; 
		*p.rddt = d1-d,d = d1; 
	}
	p.intdif.carry = d;
	return true; 
}

/*! \brief Does vasp integration/differentiation.

	\param arg argument list 
	\param dst destination vasp (NULL for in-place operation)
	\param inv true for differentiation
	\return normalized destination vasp
*/
Vasp *VaspOp::m_int(Vasp &src,const Argument &arg,Vasp *dst,BL inv) 
{ 
	Vasp *ret = NULL;
	const C *opnm = inv?"dif":"int";
	RVecBlock *vecs = GetRVecs(opnm,src,dst);
	if(vecs) {
		OpParam p; 
		p.intdif.carry = 0,p.intdif.rep = 1;
		if(arg.IsList() && arg.GetList().Count() >= 1) p.intdif.rep = flx::GetAInt(arg.GetList()[0]);
		
		if(p.intdif.rep < 0) {
			post("%s - invalid repetition count (%i) -> set to 1",opnm,p.intdif.rep);
			p.intdif.rep = 1;
		}
		
		if(p.SROvr()) {
			p.SDRRev();
			post("%s - reversing operation direction due to overlap: opposite sample delay",opnm);
		}	
		ret = DoOp(vecs,inv?VecOp::d_dif:VecOp::d_int,p);
		delete vecs;
	}
	return ret;
}


// --- find peaks

// complex peaks (radius)


inline BL higher(F a,F b) { return a > b; }
inline BL lower(F a,F b) { return a < b; }

/*! \brief Find peaks or valleys (depending on template arg)
	\param rep repetition count
	
	\todo make it functional again....
	\todo how to treat <=, >=
*/
template<BL cmp(F a,F b)>
static BL d_vlpk(OpParam &p) 
{ 
	if(p.frames >= 2) {
		for(I ir = 0; ir < p.peaks.rep; ++ir) {
			S *dst = p.rddt,*src = p.rsdt;
			register S *dd = dst,d = fabs(*dd),dn = fabs(*(src += p.rss));
			for(I i = 2; i < p.frames; ++i) { 
				if(cmp(dn,d)) *dd = 0;
				
				for(dd = dst,d = dn; (dn = fabs(*(src += p.rss))) == 0 && i < p.frames; ++i);
			}
			if(cmp(dn,d)) *dst = 0;
		}
	}
	return true; 
}

inline BL d_peaks(OpParam &p) { return d_vlpk<higher>(p); }
inline BL d_valleys(OpParam &p) { return d_vlpk<lower>(p); }

/*! \brief Finds peaks or valleys in a vasp.

	\param arg argument list 
	\param arg.rep repetition count
	\param dst destination vasp (NULL for in-place operation)
	\param inv true for valley operation
	\return normalized destination vasp
	
	\todo check for correct repetition count (int type)
*/
Vasp *VaspOp::m_peaks(Vasp &src,const Argument &arg,Vasp *dst,BL inv) 
{ 
	Vasp *ret = NULL;
	const C *opnm = inv?"valleys":"peaks";
	RVecBlock *vecs = GetRVecs(opnm,src,dst);
	if(vecs) {
		OpParam p; 
		p.peaks.rep = 1;
		if(arg.IsList() && arg.GetList().Count() >= 1) p.peaks.rep = flx::GetAInt(arg.GetList()[0]);
		
		if(p.peaks.rep < 0) {
			post("%s - invalid repetition count (%i) -> set to 1",opnm,p.peaks.rep);
			p.peaks.rep = 1;
		}
		
		if(p.SROvr()) {
			p.SDRRev();
			post("%s - reversing operation direction due to overlap: opposite sample delay",opnm);
		}	
		ret = DoOp(vecs,inv?d_peaks:d_valleys,p);
		delete vecs;
	}
	return ret;
}
