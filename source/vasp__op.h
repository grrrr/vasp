/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP__OP_H
#define __VASP__OP_H

#include "main.h"
#include <math.h>

template<class T> inline V f_rcopy(T &v,T,T b) { v = b; }

template<class T> inline V f_ccopy(T &rv,T &iv,T,T,T rb,T ib) { rv = rb,iv = ib; }

template<class T> inline V f_radd(T &v,T a,T b) { v = a+b; }
template<class T> inline V f_cadd(T &rv,T &iv,T ra,T ia,T rb,T ib) { rv = ra+rb,iv = ia+ib; }

template<class T> inline V f_rsub(T &v,T a,T b) { v = a-b; }
template<class T> inline V f_csub(T &rv,T &iv,T ra,T ia,T rb,T ib) { rv = ra-rb,iv = ia-ib; }

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

template<class T> inline V f_rmin(T &v,T a,T b) { v = a < b?a:b; }
template<class T> inline V f_cmin(T &rv,T &iv,T ra,T ia,T rb,T ib) 
{ 
	if(sqabs(ra,ia) < sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

template<class T> inline V f_rmax(T &v,T a,T b) { v = a > b?a:b; }
template<class T> inline V f_cmax(T &rv,T &iv,T ra,T ia,T rb,T ib) 
{ 
	if(sqabs(ra,ia) > sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

template<class T> inline V f_rlwr(T &v,T a,T b) { v = a < b?1:0; }
template<class T> inline V f_rgtr(T &v,T a,T b) { v = a > b?1:0; }
template<class T> inline V f_rleq(T &v,T a,T b) { v = a <= b?1:0; }
template<class T> inline V f_rgeq(T &v,T a,T b) { v = a >= b?1:0; }
template<class T> inline V f_requ(T &v,T a,T b) { v = a == b?1:0; }
template<class T> inline V f_rneq(T &v,T a,T b) { v = a != b?1:0; }

template<class T> V f_rpow(T &v,T a,T b) { v = pow(fabs(a),b)*sgn(a); } 

template<class T> inline V f_rsqr(T &v,T a) { v = a*a; } 
template<class T> inline V f_rssqr(T &v,T a) { v = a*fabs(a); } 

template<class T> inline V f_csqr(T &rv,T &iv,T ra,T ia) { rv = ra*ra-ia*ia; iv = ra*ia*2; }

template<class T> V f_rsqrt(T &v,T a) { v = sqrt(fabs(a)); } 
template<class T> V f_rssqrt(T &v,T a) { v = sqrt(fabs(a))*sgn(a); } 

template<class T> V f_rexp(T &v,T a) { v = exp(a); } 
template<class T> V f_rlog(T &v,T a) { v = log(a); }  // \todo detect NANs

template<class T> inline V f_rinv(T &v,T a) { v = 1./a; } 
template<class T> inline V f_cinv(T &rv,T &iv,T ra,T ia) 
{ 
	register R d = sqabs(ra,ia);
	rv = ra/d; iv = -ia/d; 
}

template<class T> inline V f_rabs(T &v,T a) { v = fabs(a); }  
template<class T> inline V f_rsign(T &v,T a) { v = (a == 0?0:(a < 0?-1.:1.)); }  

template<class T> V f_cnorm(T &rv,T &iv,T ra,T ia) 
{ 
	register R f = sqabs(ra,ia);
	if(f) { f = 1./sqrt(f); rv = ra*f,iv = ia*f; }
	else rv = iv = 0;
}

template<class T> V f_polar(T &rv,T &iv,T ra,T ia) { rv = sqrt(sqabs(ra,ia)),iv = arg(ra,ia); }
template<class T> V f_cart(T &rv,T &iv,T ra,T ia) { rv = ra*cos(ia),iv = ra*sin(ia); }

template<class T> inline V f_cswap(T &rv,T &iv,T ra,T ia) { rv = ia,iv = ra; }
template<class T> inline V f_cconj(T &,T &iv,T,T ia) { iv = -ia; }

template<class T> inline V f_minmax(T &rv,T &iv,T ra,T ia) 
{ 
	if(ra < ia)	rv = ra,iv = ia; 
	else rv = ia,iv = ra; 
}



#endif
