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
	if(p.part) {
		post("%s - cannot work on vector parts",p.opname);
		return false;
	}

	R v = 0;
	I i;
	for(i = 0; i < p.frames; ++i,p.rsdt += p.rss) { 
		register R s = fabs(*p.rsdt); 
		if(s > v) v = s; 
	}

	if(v && v != 1) {
		v = 1./v;
		for(i = 0; i < p.frames; ++i,p.rddt += p.rds) 
			*p.rddt = (*p.rsdt -= p.rss)*v;
	}
	return true; 
}

BL VecOp::d_copt(OpParam &p) 
{ 
	if(p.part) {
		post("%s - cannot work on vector parts",p.opname);
		return false;
	}

	R v = 0;
	I i;
	for(i = 0; i < p.frames; ++i,p.rsdt += p.rss,p.isdt += p.iss) { 
		register R s = sqabs(*p.rsdt,*p.isdt); 
		if(s > v) v = s; 
	}

	if(v && v != 1) {
		v = 1./sqrt(v);
		for(i = 0; i < p.frames; ++i,p.rddt += p.rds,p.iddt += p.ids) {
			*p.rddt = (*p.rsdt -= p.rss)*v;
			*p.iddt = (*p.isdt -= p.iss)*v;
		}
	}
	return true; 
}



BL VecOp::d_cpowi(OpParam &p) 
{ 
	const I powi = p.rbin.arg;
	if(powi < 0) {
		post("%s - negative integer power is not allowed",p.opname);
		return false;
	}

	switch(powi) {
	case 0: {
		for(; p.frames--; p.rddt += p.rds,p.iddt += p.ids) *p.rddt = 1,*p.iddt = 0;
		break;
	}
	case 1: { // just copy
		I i;
		if(p.rddt != p.rsdt)
			for(i = 0; i < p.frames; ++i,p.rsdt += p.rss,p.rddt += p.rds) *p.rddt = *p.rsdt;
		if(p.iddt != p.isdt)
			for(i = 0; i < p.frames; ++i,p.isdt += p.iss,p.iddt += p.ids) *p.iddt = *p.isdt;
		break;
	}
	case 2: {
		for(; p.frames--; p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids) 
			f_csqr(*p.rddt,*p.iddt,*p.rsdt,*p.isdt);
		break;
	}
	default: {
		for(; p.frames--; p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids) {
			register S rt,it,ra = *p.rsdt,ia = *p.isdt;
			f_csqr(rt,it,ra,ia);
			for(I i = 2; i < powi; ++i) f_cmul(rt,it,rt,it,ra,ia);
			*p.rddt = rt,*p.iddt = it;
		}
		break;
	}
	}

	return true;
}



Vasp *VaspOp::m_cpowi(Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	OpParam p("cpowi");
	if(arg.IsList() && arg.GetList().Count() >= 1) {
		CVecBlock *vecs = GetCVecs(p.opname,src,dst);
		if(vecs) {
			p.rbin.arg = flx::GetAInt(arg.GetList()[0]);
			ret = DoOp(vecs,VecOp::d_cpowi,p);
			delete vecs;
		}
	}
	return ret;
}

