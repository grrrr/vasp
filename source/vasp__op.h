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

inline V f_rcopy(S &v,S,S b) { v = b; }
inline V f_ccopy(S &rv,S &iv,S,S,S rb,S ib) { rv = rb,iv = ib; }

inline V f_radd(S &v,S a,S b) { v = a+b; }
inline V f_cadd(S &rv,S &iv,S ra,S ia,S rb,S ib) { rv = ra+rb,iv = ia+ib; }

inline V f_rsub(S &v,S a,S b) { v = a-b; }
inline V f_csub(S &rv,S &iv,S ra,S ia,S rb,S ib) { rv = ra-rb,iv = ia-ib; }

inline V f_rmul(S &v,S a,S b) { v = a*b; }
inline V f_cmul(S &rv,S &iv,S ra,S ia,S rb,S ib) { rv = ra*rb-ia*ib, iv = ra*ib+rb*ia; }

inline V f_rdiv(S &v,S a,S b) { v = a/b; }
inline V f_cdiv(S &rv,S &iv,S ra,S ia,S rb,S ib) 
{ 
	register const R den = sqabs(rb,ib);
	rv = (ra*rb+ia*ib)/den;
	iv = (ia*rb-ra*ib)/den;
}

inline V f_rmin(S &v,S a,S b) { v = a < b?a:b; }
inline V f_cmin(S &rv,S &iv,S ra,S ia,S rb,S ib) 
{ 
	if(sqabs(ra,ia) < sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

inline V f_rmax(S &v,S a,S b) { v = a > b?a:b; }
inline V f_cmax(S &rv,S &iv,S ra,S ia,S rb,S ib) 
{ 
	if(sqabs(ra,ia) > sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

inline V f_minmax(S &rv,S &iv,S ra,S ia,S rb,S ib) 
{ 
	if(sqabs(ra,ia) < sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

inline V f_rlwr(S &v,S a,S b) { v = a < b?1:0; }
inline V f_rgtr(S &v,S a,S b) { v = a > b?1:0; }

inline V f_rpow(S &v,S a,S b) { v = pow(fabs(a),b)*sgn(a); } 

inline V f_rsqr(S &v,S a) { v = a*a; } 
inline V f_rssqr(S &v,S a) { v = a*fabs(a); } 

inline V f_csqr(S &rv,S &iv,S ra,S ia) { rv = ra*ra-ia*ia; iv = ra*ia*2; }

inline V f_rsqrt(S &v,S a) { v = sqrt(fabs(a)); } 
inline V f_rssqrt(S &v,S a) { v = sqrt(fabs(a))*sgn(a); } 

inline V f_rexp(S &v,S a) { v = exp(a); } 
inline V f_rlog(S &v,S a) { v = log(a); }  // \todo detect NANs

inline V f_rinv(S &v,S a) { v = 1./a; } 
inline V f_cinv(S &rv,S &iv,S ra,S ia) 
{ 
	register R d = sqabs(ra,ia);
	rv = ra/d; iv = -ia/d; 
}

inline V f_rabs(S &v,S a) { v = fabs(a); }  
inline V f_rsign(S &v,S a) { v = (a == 0?0:(a < 0?-1.:1.)); }  

inline V f_cnorm(S &rv,S &iv,S ra,S ia) 
{ 
	register R f = sqabs(ra,ia);
	if(f) { f = 1./sqrt(f); rv = ra*f,iv = ia*f; }
	else rv = iv = 0;
}

inline V f_polar(S &rv,S &iv,S ra,S ia) { rv = sqrt(sqabs(ra,ia)),iv = arg(ra,ia); }
inline V f_cart(S &rv,S &iv,S ra,S ia) { rv = ra*cos(ia),iv = ra*sin(ia); }

inline V f_cswap(S &rv,S &iv,S ra,S ia) { rv = ia,iv = ra; }
inline V f_cconj(S &,S &iv,S,S ia) { iv = -ia; }



#endif
