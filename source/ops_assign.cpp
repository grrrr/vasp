/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_assign.h"
#include "opdefs.h"


template<class T> inline V f_rcopy(T &v,T a) { v = a; }
template<class T> inline V f_ccopy(T &rv,T &iv,T ra,T ia) { rv = ra,iv = ia; }

template<class T> inline V f_rset(T &v,T,T b) { v = b; }
template<class T> inline V f_cset(T &rv,T &iv,T,T,T rb,T ib) { rv = rb,iv = ib; }


BL VecOp::d_copy(OpParam &p) { D__run(f_rcopy<S>,p); }
BL VecOp::d_ccopy(OpParam &p) { D__cun(f_ccopy<S>,p); }

BL VecOp::d_set(OpParam &p) { D__rbin(f_rset<S>,p); }
BL VecOp::d_cset(OpParam &p) { D__cbin(f_cset<S>,p); }


VASP_BINARY("vasp.set vasp.=",set,false,VASP_ARG_R(0),"assigns a value, env or vasp")
VASP_BINARY("vasp.cset vasp.c=",cset,false,VASP_ARG_R(0),"assigns a complex value, real env or vasp")



Vasp *VaspOp::m_copy(OpParam &p,Vasp &src,Vasp &dst) 
{ 
	Vasp *ret = NULL;

	RVecBlock *vecs = GetRVecs(p.opname,src,&dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_copy,p);
		Vasp *d = vecs->DstVasp();
		if(d) dst = *d; else { dst.Clear(); delete d; }
		delete vecs;
	}

	return ret;
}

Vasp *VaspOp::m_ccopy(OpParam &p,Vasp &src,Vasp &dst) 
{ 
	Vasp *ret = NULL;
	CVecBlock *vecs = GetCVecs(p.opname,src,&dst);
	if(vecs) {
		ret = DoOp(vecs,VecOp::d_ccopy,p);
		Vasp *d = vecs->DstVasp();
		if(d) dst = *d; else { dst.Clear(); delete d; }
		delete vecs;
	}

	return ret;
}

class vasp_copy:
	public vasp_anyop
{																				
	FLEXT_HEADER(vasp_copy,vasp_anyop)
public:			
	
	vasp_copy(I argc,t_atom *argv): vasp_anyop(argc,argv,VASP_ARG(),true,XletCode(xlet::tp_any,0)) {}

	virtual V m_to(I,t_atom *) { post("s - destination vasp is ignored!",thisName()); }

	virtual Vasp *do_copy(OpParam &p,Vasp &dst) { return VaspOp::m_copy(p,ref,dst); }
		
	virtual Vasp *tx_work(const Argument &arg) 
	{ 
		OpParam p(thisName(),0);													
		
		if(arg.CanbeVasp()) {
			Vasp dst = arg.GetAVasp();
			Vasp *ret = do_copy(p,dst);
			ToOutVasp(1,dst);
			return ret;
		}
		else {
			post("%s - argument is not a valid vasp!",thisName());  // \todo check earlier!
			return NULL;
		}
	}
};																				
FLEXT_LIB_V("vasp.copy vasp.->",vasp_copy)


class vasp_ccopy:
	public vasp_copy
{																				
	FLEXT_HEADER(vasp_ccopy,vasp_copy)
public:			
	
	vasp_ccopy(I argc,t_atom *argv): vasp_copy(argc,argv) {}

	virtual Vasp *do_copy(OpParam &p,Vasp &dst) { return VaspOp::m_ccopy(p,ref,dst); }
};																				
FLEXT_LIB_V("vasp.ccopy vasp.c->",vasp_ccopy)


