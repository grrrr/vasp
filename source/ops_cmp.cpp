/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_cmp.h"
#include "opdefs.h"
#include "util.h"
#include <math.h>

// --------------------------------------------------------------

template<class T> inline V f_rlwr(T &v,T a,T b) { v = a < b?1:0; }
template<class T> inline V f_rgtr(T &v,T a,T b) { v = a > b?1:0; }
template<class T> inline V f_rleq(T &v,T a,T b) { v = a <= b?1:0; }
template<class T> inline V f_rgeq(T &v,T a,T b) { v = a >= b?1:0; }
template<class T> inline V f_requ(T &v,T a,T b) { v = a == b?1:0; }
template<class T> inline V f_rneq(T &v,T a,T b) { v = a != b?1:0; }

BL VecOp::d_lwr(OpParam &p) { D__rbin(f_rlwr<S>,p); }
BL VecOp::d_gtr(OpParam &p) { D__rbin(f_rgtr<S>,p); }
BL VecOp::d_leq(OpParam &p) { D__rbin(f_rleq<S>,p); }
BL VecOp::d_geq(OpParam &p) { D__rbin(f_rgeq<S>,p); }
BL VecOp::d_equ(OpParam &p) { D__rbin(f_requ<S>,p); }
BL VecOp::d_neq(OpParam &p) { D__rbin(f_rneq<S>,p); }

VASP_BINARY("vasp.<",lwr,true,VASP_ARG_R(0),"")
VASP_BINARY("vasp.>",gtr,true,VASP_ARG_R(0),"")
VASP_BINARY("vasp.<=",leq,true,VASP_ARG_R(0),"")
VASP_BINARY("vasp.>=",geq,true,VASP_ARG_R(0),"")
VASP_BINARY("vasp.==",equ,true,VASP_ARG_R(0),"")
VASP_BINARY("vasp.!=",neq,true,VASP_ARG_R(0),"")


// --------------------------------------------------------------

template<class T> inline V f_min(T &v,T a,T b) { v = a < b?a:b; }
template<class T> inline V f_max(T &v,T a,T b) { v = a > b?a:b; }

template<class T> inline V f_rmin(T &rv,T &iv,T ra,T ia,T rb,T ib) 
{ 
	if(sqabs(ra,ia) < sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

template<class T> inline V f_rmax(T &rv,T &iv,T ra,T ia,T rb,T ib) 
{ 
	if(sqabs(ra,ia) > sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

BL VecOp::d_min(OpParam &p) { D__rbin(f_min<S>,p); }
BL VecOp::d_max(OpParam &p) { D__rbin(f_max<S>,p); }
BL VecOp::d_rmin(OpParam &p) { d__cbin(f_rmin<S>,p); }
BL VecOp::d_rmax(OpParam &p) { d__cbin(f_rmax<S>,p); }


VASP_BINARY("vasp.min",min,true,VASP_ARG_R(0),"assigns the minimum of the comparison with a value or vasp")
VASP_BINARY("vasp.max",max,true,VASP_ARG_R(0),"assigns the maximum of the comparison with a value or vasp")

VASP_BINARY("vasp.rmin",rmin,true,VASP_ARG_R(0),"assigns the minimum of the radius comparison with a complex value or vasp")
VASP_BINARY("vasp.rmax",rmax,true,VASP_ARG_R(0),"assigns the maximum of the radius comparison with a complex value or vasp")


// --------------------------------------------------------------

template<class T> V f_maxq(T &,T ra,OpParam &p) 
{ 
	if(ra > p.norm.minmax) p.norm.minmax = ra; 
} 

template<class T> V f_minq(T &,T ra,OpParam &p) 
{ 
	if(ra < p.norm.minmax) p.norm.minmax = ra; 
} 

template<class T> V f_amaxq(T &,T ra,OpParam &p) 
{ 
	register T s = fabs(ra); 
	if(s > p.norm.minmax) p.norm.minmax = s; 
} 

template<class T> V f_aminq(T &,T ra,OpParam &p) 
{ 
	register T s = fabs(ra); 
	if(s < p.norm.minmax) p.norm.minmax = s; 
} 

template<class T> V f_rmaxq(T &,T &,T ra,T ia,OpParam &p) 
{ 
	register T s = sqabs(ra,ia); 
	if(s > p.norm.minmax) p.norm.minmax = s; 
} 

template<class T> V f_rminq(T &,T &,T ra,T ia,OpParam &p) 
{ 
	register T s = sqabs(ra,ia); 
	if(s < p.norm.minmax) p.norm.minmax = s; 
} 

BL VecOp::d_minq(OpParam &p) { D__rop(f_minq<S>,p); }
BL VecOp::d_maxq(OpParam &p) { D__rop(f_maxq<S>,p); }
BL VecOp::d_aminq(OpParam &p) { D__rop(f_aminq<S>,p); }
BL VecOp::d_amaxq(OpParam &p) { D__rop(f_amaxq<S>,p); }
BL VecOp::d_rminq(OpParam &p) { d__cop(f_rminq<S>,p); }
BL VecOp::d_rmaxq(OpParam &p) { d__cop(f_rmaxq<S>,p); }


// --------------------------------------------------------------

template<class T> V f_gate(T &rv,T ra,T rb) { rv = fabs(ra) >= rb?ra:0; } 
template<class T> V f_igate(T &rv,T ra,T rb) { rv = fabs(ra) <= rb?ra:0; } 

template<class T> V f_rgate(T &rv,T &iv,T ra,T ia,T rb,T) 
{ 
	register const T _abs = sqabs(ra,ia);

	if(_abs >= rb*rb) rv = ra,iv = ia;
	else rv = iv = 0;
} 

template<class T> V f_rigate(T &rv,T &iv,T ra,T ia,T rb,T) 
{ 
	register const T _abs = sqabs(ra,ia);

	if(_abs <= rb*rb) rv = ra,iv = ia;
	else rv = iv = 0;
} 

BL VecOp::d_gate(OpParam &p) { D__rbin(f_gate<S>,p); }
BL VecOp::d_igate(OpParam &p) { d__rbin(f_igate<S>,p); }
BL VecOp::d_rgate(OpParam &p) { d__cbin(f_rgate<S>,p); }
BL VecOp::d_rigate(OpParam &p) { d__cbin(f_rigate<S>,p); }



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

VASP_ANYOP("vasp.gate",gate,1,true,VASP_ARG_R(1),"") 
//VASP_ANYOP("vasp.!gate",igate,1,true,"") 
VASP_ANYOP("vasp.rgate",rgate,1,true,VASP_ARG_R(1),"") 
//VASP_ANYOP("vasp.r!gate",rigate,1,true,"") 

// --------------------------------------------------------------

template<class T> inline V f_minmax(T &rv,T &iv,T ra,T ia) 
{ 
	if(ra < ia)	rv = ra,iv = ia; 
	else rv = ia,iv = ra; 
} 

BL VecOp::d_minmax(OpParam &p) { d__cun(f_minmax<S>,p); }

VASP_UNARY("vasp.minmax",minmax,true,"") 


// --------------------------------------------------------------



/*! \class vasp_qmin
	\remark \b vasp.min?
	\brief Get minimum sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qmin:
	public vasp_unop
{
	FLEXT_HEADER(vasp_qmin,vasp_unop)

public:
	vasp_qmin(): vasp_unop(true,XletCode(xlet::tp_float,0)) {}

	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = BIG;
		Vasp *ret = VaspOp::m_qmin(p,ref); 
		if(p.norm.minmax == BIG) p.norm.minmax = 0;
		return ret;
	}
		
	virtual Vasp *tx_work() 
	{ 
		OpParam p(thisName(),0);													
		Vasp *ret = do_opt(p);
		ToOutFloat(1,p.norm.minmax);
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's minimum sample value",thisName()); }
};

FLEXT_LIB("vasp.min?",vasp_qmin)


/*! \class vasp_qamin
	\remark \b vasp.amin?
	\brief Get minimum absolute sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qamin:
	public vasp_qmin
{
	FLEXT_HEADER(vasp_qamin,vasp_qmin)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = BIG;
		Vasp *ret = VaspOp::m_qamin(p,ref); 
		if(p.norm.minmax == BIG) p.norm.minmax = 0;
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's minimum absolute sample value",thisName()); }
};

FLEXT_LIB("vasp.amin?",vasp_qamin)



/*! \class vasp_qmax
	\remark \b vasp.max?
	\brief Get maximum sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qmax:
	public vasp_qmin
{
	FLEXT_HEADER(vasp_qmax,vasp_qmin)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = -BIG;
		Vasp *ret = VaspOp::m_qmax(p,ref); 
		if(p.norm.minmax == -BIG) p.norm.minmax = 0;
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's maximum sample value",thisName()); }
};

FLEXT_LIB("vasp.max?",vasp_qmax)



/*! \class vasp_qamax
	\remark \b vasp.amax?
	\brief Get minimum absolute sample value
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qamax:
	public vasp_qmax
{
	FLEXT_HEADER(vasp_qamax,vasp_qmax)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = 0;
		return VaspOp::m_qamax(p,ref); 
	}

	virtual V m_help() { post("%s - Get a vasp's maximum absolute sample value",thisName()); }
};

FLEXT_LIB("vasp.amax?",vasp_qamax)




/*! \class vasp_qrmin
	\remark \b vasp.rmin?
	\brief Get minimum complex radius of samples
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qrmin:
	public vasp_unop
{
	FLEXT_HEADER(vasp_qrmin,vasp_unop)

public:
	vasp_qrmin(): vasp_unop(true,XletCode(xlet::tp_float,0)) {}

	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = BIG;
		Vasp *ret = VaspOp::m_qrmin(p,ref); 
		if(p.norm.minmax == BIG) p.norm.minmax = 0;
		return ret;
	}
		
	virtual Vasp *tx_work() 
	{ 
		OpParam p(thisName(),0);													
		Vasp *ret = do_opt(p);
		ToOutFloat(1,sqrt(p.norm.minmax));
		return ret;
	}

	virtual V m_help() { post("%s - Get a vasp's minimum complex radius",thisName()); }
};

FLEXT_LIB("vasp.rmin?",vasp_qrmin)



/*! \class vasp_qrmax
	\remark \b vasp.rmax?
	\brief Get maximum complex radius of samples
	\since 0.0.2
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet float - minimum sample value

	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
	\remark Returns 0 for a vasp with 0 frames
*/
class vasp_qrmax:
	public vasp_qrmin
{
	FLEXT_HEADER(vasp_qrmax,vasp_qrmin)
public:
	virtual Vasp *do_opt(OpParam &p) 
	{ 
		p.norm.minmax = 0;
		return VaspOp::m_qrmax(p,ref); 
	}

	virtual V m_help() { post("%s - Get a vasp's maximum complex radius",thisName()); }
};

FLEXT_LIB("vasp.rmax?",vasp_qrmax)



