/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vasp__op.h"

#include <math.h>


Vasp *VaspOp::m_opt(OpParam &p,Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		p.norm.fnorm = 0;
		ret = DoOp(vecs,VecOp::d_optq,p);
		if(ret) {
			delete ret;

			if(p.norm.fnorm && p.norm.fnorm != 1) {
				R f = p.norm.fnorm; // save for later
				p.rbin.arg = 1/f;
				ret = DoOp(vecs,VecOp::d_mul,p);
				p.norm.fnorm = f;  // return to object
			}
			else
				ret = DoOp(vecs,VecOp::d_copy,p);
		}
		delete vecs;
	}
	return ret;
}


//! \todo opt could be replaced by *
Vasp *VaspOp::m_optf(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	post("%s - sorry, not implemented yet",p.opname);
	return NULL;

	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.norm.scl = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - scaling factor is invalid -> set to 1",p.opname);
			p.norm.scl = 1;
		}

		p.norm.fnorm = 0;
		ret = DoOp(vecs,VecOp::d_optq,p);
		if(ret) {
			delete ret;

			if(p.norm.fnorm)
				ret = DoOp(vecs,VecOp::d_optf,p);
			else
				ret = DoOp(vecs,VecOp::d_copy,p);
		}
		delete vecs;
	}
	return ret;
}



class vasp_opt:
	public vasp_unop
{																				
	FLEXT_HEADER(vasp_opt,vasp_unop)
public:			
	
	vasp_opt():	vasp_unop(true,1) {}

	virtual Vasp *do_opt(OpParam &p) { return VaspOp::m_opt(p,ref,&dst); }
		
	virtual Vasp *tx_work() 
	{ 
		OpParam p(thisName());													
		Vasp *ret = do_opt(p);
		ToOutFloat(1,p.norm.fnorm);
		return ret;
	}
};																				
FLEXT_LIB("vasp.opt",vasp_opt)



Vasp *VaspOp::m_ropt(OpParam &p,Vasp &src,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		p.norm.fnorm = 0;
		ret = DoOp(vecs,VecOp::d_roptq,p);
		if(ret) {
			delete ret;

			if(p.norm.fnorm && p.norm.fnorm != 1) {
				R f = sqrt(p.norm.fnorm); // save for later
				p.rbin.arg = 1/f;
				ret = DoOp(vecs,VecOp::d_cmul,p);
				p.norm.fnorm = f;  // return to object
			}
			else
				ret = DoOp(vecs,VecOp::d_ccopy,p);
		}
		delete vecs;
	}
	return ret;
}


class vasp_ropt: public vasp_opt	
{																				
	FLEXT_HEADER(vasp_ropt,vasp_opt)
public:			
	virtual Vasp *do_opt(OpParam &p) { return VaspOp::m_ropt(p,ref,&dst); }
};																				
FLEXT_LIB("vasp.ropt",vasp_ropt)



Vasp *VaspOp::m_roptf(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	post("%s - sorry, not implemented yet",p.opname);
	return NULL;

	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.norm.scl = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - scaling factor is invalid -> set to 1",p.opname);
			p.norm.scl = 1;
		}

		p.norm.fnorm = 0;
		ret = DoOp(vecs,VecOp::d_roptq,p);
		if(ret) {
			delete ret;

			if(p.norm.fnorm) 
				ret = DoOp(vecs,VecOp::d_roptf,p);
			else
				ret = DoOp(vecs,VecOp::d_ccopy,p);
		}
		delete vecs;
	}
	return ret;
}








Vasp *VaspOp::m_cpowi(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		I powi = 1;
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeInt(arg.GetList()[0]))
			powi = flx::GetAInt(arg.GetList()[0]);
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



Vasp *VaspOp::m_rpow(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 1",p.opname);
			p.cbin.rarg = 1;
		}
		p.cbin.iarg = 0; // not used anyway

		ret = DoOp(vecs,VecOp::d_rpow,p);
		delete vecs;
	}
	return ret;
}


Vasp *VaspOp::m_radd(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 0",p.opname);
			p.cbin.rarg = 0;
		}
		p.cbin.iarg = 0; // not used anyway

		ret = DoOp(vecs,VecOp::d_radd,p);
		delete vecs;
	}
	return ret;
}


Vasp *VaspOp::m_gate(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.rbin.arg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 1",p.opname);
			p.rbin.arg = 1;
		}

		ret = DoOp(vecs,VecOp::d_gate,p);
		delete vecs;
	}
	return ret;
}


Vasp *VaspOp::m_rgate(OpParam &p,Vasp &src,const Argument &arg,Vasp *dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		if(arg.IsList() && arg.GetList().Count() >= 1 && flx::CanbeFloat(arg.GetList()[0]))
			p.cbin.rarg = flx::GetAFloat(arg.GetList()[0]);
		else {
			post("%s - argument is invalid -> set to 1",p.opname);
			p.cbin.rarg = 1;
		}
		p.cbin.iarg = 0; // not used anyway

		ret = DoOp(vecs,VecOp::d_rgate,p);
		delete vecs;
	}
	return ret;
}

