/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_arith.h"
#include "opdefs.h"

template<class T> inline V f_radd(T &v,T a,T b) { v = a+b; }
template<class T> inline V f_cadd(T &rv,T &iv,T ra,T ia,T rb,T ib) { rv = ra+rb,iv = ia+ib; }

template<class T> inline V f_rsub(T &v,T a,T b) { v = a-b; }
template<class T> inline V f_csub(T &rv,T &iv,T ra,T ia,T rb,T ib) { rv = ra-rb,iv = ia-ib; }

template<class T> inline V f_rsubr(T &v,T a,T b) { v = b-a; }
template<class T> inline V f_csubr(T &rv,T &iv,T ra,T ia,T rb,T ib) { rv = rb-ra,iv = ib-ia; }

template<class T> inline V f_rmul(T &v,T a,T b) { v = a*b; }
template<class T> inline V f_cmul(T &rv,T &iv,T ra,T ia,T rb,T ib) { rv = ra*rb-ia*ib, iv = ra*ib+rb*ia; }

template<class T> inline V f_rdiv(T &v,T a,T b) { v = a/b; }
template<class T> inline V f_cdiv(T &rv,T &iv,T ra,T ia,T rb,T ib) 
{ 
	register const R den = sqabs(rb,ib);
	rv = (ra*rb+ia*ib)/den;
	iv = (ia*rb-ra*ib)/den;
}

template<class T> inline V f_rdivr(T &v,T a,T b) { v = b/a; }
template<class T> inline V f_cdivr(T &rv,T &iv,T ra,T ia,T rb,T ib)
{ 
	register const R den = sqabs(ra,ia);
	rv = (rb*ra+ib*ia)/den;
	iv = (ib*ra-rb*ia)/den;
}

template<class T> inline V f_rmod(T &v,T a,T b) { v = fmod(a,b); }

BL VecOp::d_add(OpParam &p) { D__rbin(f_radd<S>,p); }
BL VecOp::d_cadd(OpParam &p) { D__cbin(f_cadd<S>,p); }
BL VecOp::d_sub(OpParam &p) { D__rbin(f_rsub<S>,p); }
BL VecOp::d_csub(OpParam &p) { D__cbin(f_csub<S>,p); }
BL VecOp::d_subr(OpParam &p) { D__rbin(f_rsubr<S>,p); }
BL VecOp::d_csubr(OpParam &p) { D__cbin(f_csubr<S>,p); }
BL VecOp::d_mul(OpParam &p) { D__rbin(f_rmul<S>,p); }
BL VecOp::d_cmul(OpParam &p) { D__cbin(f_cmul<S>,p); }
BL VecOp::d_div(OpParam &p) { D__rbin(f_rdiv<S>,p); }
BL VecOp::d_cdiv(OpParam &p) { return d__cbin(f_cdiv<S>,p); }
BL VecOp::d_divr(OpParam &p) { D__rbin(f_rdivr<S>,p); }
BL VecOp::d_cdivr(OpParam &p) { return d__cbin(f_cdivr<S>,p); }
BL VecOp::d_mod(OpParam &p) { D__rbin(f_rmod<S>,p); }

template<class T> inline V f_rsign(T &v,T a) { v = (a == 0?0:(a < 0?-1.:1.)); }  
template<class T> inline V f_rabs(T &v,T a) { v = fabs(a); }  
template<class T> inline V f_cabs(T &rv,T &iv,T ra,T ia) { rv = sqrt(ra*ra+ia*ia),iv = 0; }

BL VecOp::d_sign(OpParam &p) { D__run(f_rsign<S>,p); }
BL VecOp::d_abs(OpParam &p) { D__run(f_rabs<S>,p); }
BL VecOp::d_cabs(OpParam &p) { D__cun(f_cabs<S>,p); }

