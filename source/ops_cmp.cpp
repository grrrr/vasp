/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_cmp.h"
#include "opdefs.h"

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
BL VecOp::d_rmin(OpParam &p) { return d__cbin(f_rmin<S>,p); }
BL VecOp::d_rmax(OpParam &p) { return d__cbin(f_rmax<S>,p); }


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
BL VecOp::d_rminq(OpParam &p) { return d__cop(f_rminq<S>,p); }
BL VecOp::d_rmaxq(OpParam &p) { return d__cop(f_rmaxq<S>,p); }


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
BL VecOp::d_igate(OpParam &p) { return d__rbin(f_igate<S>,p); }
BL VecOp::d_rgate(OpParam &p) { return d__cbin(f_rgate<S>,p); }
BL VecOp::d_rigate(OpParam &p) { return d__cbin(f_rigate<S>,p); }


template<class T> inline V f_minmax(T &rv,T &iv,T ra,T ia) 
{ 
	if(ra < ia)	rv = ra,iv = ia; 
	else rv = ia,iv = ra; 
}

BL VecOp::d_minmax(OpParam &p) { return d__cun(f_minmax<S>,p); }

