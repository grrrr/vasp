/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_trnsc.h"
#include "opdefs.h"

// --------------------------------------------------------------

template<class T> V f_rpow(T &v,T a,T b) { v = pow(fabs(a),b)*sgn(a); } 

BL VecOp::d_pow(OpParam &p) { return d__rbin(f_rpow<S>,p); }

template<class T> V f_crpow(T &rv,T &iv,T ra,T ia,T rb,T) 
{ 
	register const R _abs = sqrt(sqabs(ra,ia));
	if(_abs) {
		register const R _p = pow(_abs,rb)/_abs;
		rv = _p*ra,iv = _p*ia;
	}
	else
		rv = iv = 0;
} 

template<class T> V f_cpowi(T &rv,T &iv,T ra,T ia,OpParam &p) 
{ 
	register const I powi = p.ibin.arg;
	register S rt,it; f_csqr(rt,it,ra,ia);
	for(I i = 2; i < powi; ++i) f_cmul(rt,it,rt,it,ra,ia);
	rv = rt,iv = it;
} 

BL VecOp::d_rpow(OpParam &p) { return d__cbin(f_crpow<S>,p); }
BL VecOp::d_cpowi(OpParam &p) { return d__cop(f_cpowi<S>,p); }


template<class T> inline V f_rsqr(T &v,T a) { v = a*a; } 
template<class T> inline V f_rssqr(T &v,T a) { v = a*fabs(a); } 

template<class T> V f_rsqrt(T &v,T a) { v = sqrt(fabs(a)); } 
template<class T> V f_rssqrt(T &v,T a) { v = sqrt(fabs(a))*sgn(a); } 

BL VecOp::d_sqr(OpParam &p) { D__run(f_rsqr<S>,p); }
BL VecOp::d_ssqr(OpParam &p) { return d__run(f_rssqr<S>,p); }
BL VecOp::d_sqrt(OpParam &p) { return d__run(f_rsqrt<S>,p); }
BL VecOp::d_ssqrt(OpParam &p) { return d__run(f_rssqrt<S>,p); }


template<class T> inline V f_csqr(T &rv,T &iv,T ra,T ia) { rv = ra*ra-ia*ia; iv = ra*ia*2; }

BL VecOp::d_csqr(OpParam &p) { D__cun(f_csqr<S>,p); }


template<class T> V f_rexp(T &v,T a) { v = exp(a); } 
template<class T> V f_rlog(T &v,T a) { v = log(a); }  // \todo detect NANs

BL VecOp::d_exp(OpParam &p) { return d__run(f_rexp<S>,p); }
BL VecOp::d_log(OpParam &p) { return d__run(f_rlog<S>,p); }



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
				p.ibin.arg = powi;
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

