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


template<class T> inline V f_rcopy(T &v,T a) { v = a; }
template<class T> inline V f_ccopy(T &rv,T &iv,T ra,T ia) { rv = ra,iv = ia; }

template<class T> inline V f_rset(T &v,T,T b) { v = b; }
template<class T> inline V f_cset(T &rv,T &iv,T,T,T rb,T ib) { rv = rb,iv = ib; }

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

template<class T> inline V f_min(T &v,T a,T b) { v = a < b?a:b; }
template<class T> inline V f_rmin(T &rv,T &iv,T ra,T ia,T rb,T ib) 
{ 
	if(sqabs(ra,ia) < sqabs(rb,ib))	rv = ra,iv = ia; 
	else rv = rb,iv = ib; 
}

template<class T> inline V f_max(T &v,T a,T b) { v = a > b?a:b; }
template<class T> inline V f_rmax(T &rv,T &iv,T ra,T ia,T rb,T ib) 
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

template<class T> V f_cpowi(T &rv,T &iv,T ra,T ia,T rb,T) 
{ 
	register const I powi = rb;
	register S rt,it; f_csqr(rt,it,ra,ia);
	for(I i = 2; i < powi; ++i) f_cmul(rt,it,rt,it,ra,ia);
	rv = rt,iv = it;
} 

template<class T> V f_radd(T &rv,T &iv,T ra,T ia,T rb,T) 
{ 
	register const R _abs = sqrt(sqabs(ra,ia))+rb;
	register const R _phi = arg(ra,ia);

	rv = _abs*cos(_phi),iv = _abs*sin(_phi);
} 

template<class T> V f_gate(T &rv,T ra,T rb) { rv = fabs(ra) >= rb?ra:0; } 

template<class T> V f_rgate(T &rv,T &iv,T ra,T ia,T rb,T) 
{ 
	register const T _abs = sqabs(ra,ia);

	if(_abs >= rb*rb) rv = ra,iv = ia;
	else rv = iv = 0;
} 

template<class T> V f_optq(T &,T ra,OpParam &p) 
{ 
	register T s = fabs(ra); 
	if(s > p.norm.fnorm) p.norm.fnorm = s; 
} 

template<class T> V f_roptq(T &,T &,T ra,T ia,OpParam &p) 
{ 
	register T s = sqabs(ra,ia); 
	if(s > p.norm.fnorm) p.norm.fnorm = s; 
} 

template<class T> V f_optf(T &rv,T ra,OpParam &p) 
{ 
} 

template<class T> V f_roptf(T &rv,T &iv,T ra,T ia,OpParam &p) 
{ 
} 


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
	register T d = sqabs(ra,ia);
	rv = ra/d; iv = -ia/d; 
}

template<class T> inline V f_rabs(T &v,T a) { v = fabs(a); }  
template<class T> inline V f_rsign(T &v,T a) { v = (a == 0?0:(a < 0?-1.:1.)); }  

template<class T> V f_cnorm(T &rv,T &iv,T ra,T ia) 
{ 
	register T f = sqabs(ra,ia);
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





/*! \brief skeleton for unary real operations
	\todo optimization for src=dst
*/
#define D__run(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(sr == dr)												\
		if(p.rds == 1)											\
			for(I i = 0; i < p.frames; ++i,dr++)				\
				fun(*dr,*dr);									\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds)			\
				fun(*dr,*dr);									\
	else														\
		if(p.rss == 1 && p.rds == 1)							\
			for(I i = 0; i < p.frames; ++i,sr++,dr++)			\
				fun(*dr,*sr);									\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds)	\
				fun(*dr,*sr);										\
	return true;												\
}

/*! \brief skeleton for unary complex operations
	\todo optimization for src=dst
*/
#define D__cun(fun,p)											\
{																\
	register const S *sr = p.rsdt,*si = p.isdt;					\
	register S *dr = p.rddt,*di = p.iddt;						\
	if(sr == dr && si == di)									\
		if(p.rds == 1 && p.ids == 1)							\
			for(I i = 0; i < p.frames; ++i,dr++,di++)			\
				fun(*dr,*di,*dr,*di);							\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*dr,*di);								\
	else														\
		if(p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1) \
			for(I i = 0; i < p.frames; ++i,sr++,si++,dr++,di++) \
				fun(*dr,*di,*sr,*si);								\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*sr,*si);								\
	return true;												\
}

/*! \brief skeleton for binary real operations
	\todo optimization for src=dst
*/
#define D__rbin(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(p.HasArg()) {											\
		register const S *ar = p.radt;							\
		if(p.rsdt == p.rddt)									\
			if(p.rds == 1 && p.ras == 1)						\
				for(I i = 0; i < p.frames; ++i,dr++,ar++)		\
					fun(*dr,*dr,*ar);							\
			else												\
				for(I i = 0; i < p.frames; ++i,dr += p.rds,ar += p.ras) \
					fun(*dr,*dr,*ar);							\
		else													\
			if(p.rss == 1 && p.rds == 1 && p.ras == 1)			\
				for(I i = 0; i < p.frames; ++i,sr++,dr++,ar++)  \
					fun(*dr,*sr,*ar);							\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds,ar += p.ras) \
					fun(*dr,*sr,*ar);							\
	}															\
	else {														\
		register const S v = p.rbin.arg;						\
		if(p.rsdt == p.rddt)									\
			if(p.rds == 1)										\
				for(I i = 0; i < p.frames; ++i,dr++)			\
					fun(*dr,*dr,v);								\
			else												\
				for(I i = 0; i < p.frames; ++i,dr += p.rds)		\
					fun(*dr,*dr,v);								\
		else													\
			if(p.rss == 1 && p.rds == 1)						\
				for(I i = 0; i < p.frames; ++i,sr++,dr++)		\
					fun(*dr,*sr,v);								\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds) \
					fun(*dr,*sr,v);								\
	}															\
	return true;												\
}

/*! \brief skeleton for binary complex operations
	\todo optimization for src=dst
*/
#define D__cbin(fun,p)											\
{																\
	register const S *sr = p.rsdt,*si = p.isdt;					\
	register S *dr = p.rddt,*di = p.iddt;						\
	if(p.HasArg()) {											\
		register const S *ar = p.radt,*ai = p.iadt;				\
		if(ai)													\
			if(sr == dr && si == di)							\
				if(p.rds == 1 && p.ids == 1 && p.ras == 1 && p.ias == 1) \
					for(I i = 0; i < p.frames; ++i,dr++,di++,ar++,ai++) \
						fun(*dr,*di,*dr,*di,*ar,*ai);			\
				else											\
					for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,ar += p.ras,ai += p.ias) \
						fun(*dr,*di,*dr,*di,*ar,*ai);			\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.ras,ai += p.ias) \
					fun(*dr,*di,*sr,*si,*ar,*ai);				\
		else													\
			if(sr == dr && si == di)							\
				for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,ar += p.ras) \
					fun(*dr,*di,*dr,*di,*ar,0);					\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.ras) \
					fun(*dr,*di,*sr,*si,*ar,0);					\
	}															\
	else {														\
		register const S rv = p.cbin.rarg,iv = p.cbin.iarg;		\
		if(sr == dr && si == di)								\
			if(p.rds == 1 && p.ids == 1)						\
				for(I i = 0; i < p.frames; ++i,dr++,di++)		\
					fun(*dr,*di,*dr,*di,rv,iv);					\
			else												\
				for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
					fun(*dr,*di,*dr,*di,rv,iv);					\
		else													\
			if(p.rds == 1 && p.ids == 1 && p.rss == 1 && p.iss == 1) \
				for(I i = 0; i < p.frames; ++i,sr++,si++,dr++,di++) \
					fun(*dr,*di,*sr,*si,rv,iv);					\
			else												\
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
					fun(*dr,*di,*sr,*si,rv,iv);					\
	}															\
	return true;												\
}


/*! \brief skeleton for unary real operations
	\todo optimization for src=dst
*/
#define D__rop(fun,p)											\
{																\
	register const S *sr = p.rsdt;								\
	register S *dr = p.rddt;									\
	if(sr == dr)												\
		if(p.rds == 1)											\
			for(I i = 0; i < p.frames; ++i,dr++)				\
				fun(*dr,*dr,p);									\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds)			\
				fun(*dr,*dr,p);									\
	else														\
		if(p.rss == 1 && p.rds == 1)							\
			for(I i = 0; i < p.frames; ++i,sr++,dr++)			\
				fun(*dr,*sr,p);									\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds)	\
				fun(*dr,*sr,p);										\
	return true;												\
}

/*! \brief skeleton for unary complex operations
	\todo optimization for src=dst
*/
#define D__cop(fun,p)											\
{																\
	register const S *sr = p.rsdt,*si = p.isdt;					\
	register S *dr = p.rddt,*di = p.iddt;						\
	if(sr == dr && si == di)									\
		if(p.rds == 1 && p.ids == 1)							\
			for(I i = 0; i < p.frames; ++i,dr++,di++)			\
				fun(*dr,*di,*dr,*di,p);							\
		else													\
			for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*dr,*di,p);								\
	else														\
		if(p.rss == 1 && p.iss == 1 && p.rds == 1 && p.ids == 1) \
			for(I i = 0; i < p.frames; ++i,sr++,si++,dr++,di++) \
				fun(*dr,*di,*sr,*si,p);								\
		else													\
			for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) \
				fun(*dr,*di,*sr,*si,p);								\
	return true;												\
}



#endif
