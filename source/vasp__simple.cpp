/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vasp__op.h"

/*
#ifdef _MSC_VER

#define TEMPL1(V1) 
#define TINIT1(L1,V1) const I L1 = _##L1
#define TF1(FUN) FUN

#define TEMPL2(V1,V2) 
#define TINIT2(L1,V1,L2,V2) const I L1 = _##L1,L2 = _##L2
#define TF2(FUN) FUN

#define TEMPL3(V1,V2,V3) 
#define TINIT3(L1,V1,L2,V2,L3,V3) const I L1 = _##L1,L2 = _##L2,L3 = _##L3
#define TF3(FUN) FUN

#else

// template optimization on
#define TEMPL1(V1) template <I V1>
#define TINIT1(L1,V1) const I L1 = V1?V1:_##L1
#define TF1(FUN) FUN<0>

#define TEMPL2(V1,V2) template <I V1,I V2>
#define TINIT2(L1,V1,L2,V2) const I L1 = V1?V1:_##L1,L2 = V2?V2:_##L2
#define TF2(FUN) FUN<0,0>

#define TEMPL3(V1,V2,V3) template <I V1,I V2,I V3>
#define TINIT2(L1,V1,L2,V2,L3,V3) const I L1 = V1?V1:_##L1,L2 = V2?V2:_##L2,L3 = V3?V3:_##L3
#define TF3(FUN) FUN<0,0,0>

#endif
*/

#if 0

/*! \brief skeleton for unary real operations
	\todo optimization for src=dst
*/
template <V fun(S &v,S a)>
static BL d__run(OpParam &p) 
{ 
	register const S *sr = p.rsdt;
	register S *dr = p.rddt;
	if(sr == dr)
		for(I i = 0; i < p.frames; ++i,dr += p.rds) 
			fun(*dr,*dr); 
	else
		for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds) 
			fun(*dr,*sr); 
	return true; 
}

/*! \brief skeleton for unary complex operations
	\todo optimization for src=dst
*/
template <V fun(S &rv,S &iv,S ra,S ia)>
static BL d__cun(OpParam &p) 
{ 
	register const S *sr = p.rsdt,*si = p.isdt;
	register S *dr = p.rddt,*di = p.iddt;
	if(sr == dr && si == di)
		for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) 
			fun(*dr,*di,*dr,*di);
	else
		for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) 
			fun(*dr,*di,*sr,*si);
	return true; 
}

/*! \brief skeleton for binary real operations
	\todo optimization for src=dst
*/
template <V fun(S &v,S a,S b)>
static BL d__rbin(OpParam &p) 
{ 
	register const S *sr = p.rsdt;
	register S *dr = p.rddt;
	if(p.HasArg()) {
		register const S *ar = p.radt;
		if(p.rsdt == p.rddt)
			for(I i = 0; i < p.frames; ++i,dr += p.rds,ar += p.ras) 
				fun(*dr,*dr,*ar); 
		else
			for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds,ar += p.ras) 
				fun(*dr,*sr,*ar); 
	}
	else {
		register const S v = p.rbin.arg;
		if(p.rsdt == p.rddt)
			for(I i = 0; i < p.frames; ++i,dr += p.rds) 
				fun(*dr,*dr,v); 
		else
			for(I i = 0; i < p.frames; ++i,sr += p.rss,dr += p.rds) 
				fun(*dr,*sr,v); 
	}

	return true; 
}

/*! \brief skeleton for binary complex operations
	\todo optimization for src=dst
*/
template <V fun(S &rv,S &iv,S ra,S ia,S rb,S ib)>
static BL d__cbin(OpParam &p) 
{ 
	register const S *sr = p.rsdt,*si = p.isdt;
	register S *dr = p.rddt,*di = p.iddt;
	if(p.HasArg()) {
		register const S *ar = p.radt,*ai = p.iadt;
		if(ai) 
			// imaginary part exists
			if(sr == dr && si == di)
				for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,ar += p.ras,ai += p.ias) 
					fun(*dr,*di,*dr,*di,*ar,*ai);
			else
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.ras,ai += p.ias) 
					fun(*dr,*di,*sr,*si,*ar,*ai);
		else
			// no imaginary part
			if(sr == dr && si == di)
				for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids,ar += p.ras) 
					fun(*dr,*di,*dr,*di,*ar,0);
			else
				for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids,ar += p.ras) 
					fun(*dr,*di,*sr,*si,*ar,0);
	}
	else {
		register const S rv = p.cbin.rarg,iv = p.cbin.iarg;
		if(sr == dr && si == di)
			for(I i = 0; i < p.frames; ++i,dr += p.rds,di += p.ids) 
				fun(*dr,*di,*dr,*di,rv,iv);
		else
			for(I i = 0; i < p.frames; ++i,sr += p.rss,si += p.iss,dr += p.rds,di += p.ids) 
				fun(*dr,*di,*sr,*si,rv,iv);
	}

	return true; 
}

#define D__run(fun,p) return d__run< fun >(p);
#define D__cun(fun,p) return d__cun< fun >(p);
#define D__rbin(fun,p) return d__rbin< fun >(p);
#define D__cbin(fun,p) return d__cbin< fun >(p);


#else

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

#endif

// --------------------------------------------------------------

BL VecOp::d_copy(OpParam &p) { D__rbin(f_rcopy<S>,p); }
BL VecOp::d_ccopy(OpParam &p) { D__cbin(f_ccopy<S>,p); }
BL VecOp::d_add(OpParam &p) { D__rbin(f_radd<S>,p); }
BL VecOp::d_cadd(OpParam &p) { D__cbin(f_cadd<S>,p); }
BL VecOp::d_sub(OpParam &p) { D__rbin(f_rsub<S>,p); }
BL VecOp::d_csub(OpParam &p) { D__cbin(f_csub<S>,p); }
BL VecOp::d_mul(OpParam &p) { D__rbin(f_rmul<S>,p); }
BL VecOp::d_cmul(OpParam &p) { D__cbin(f_cmul<S>,p); }
BL VecOp::d_div(OpParam &p) { D__rbin(f_rdiv<S>,p); }
BL VecOp::d_cdiv(OpParam &p) { D__cbin(f_cdiv<S>,p); }
BL VecOp::d_divr(OpParam &p) { D__rbin(f_rdivr<S>,p); }
BL VecOp::d_cdivr(OpParam &p) { D__cbin(f_cdivr<S>,p); }
BL VecOp::d_mod(OpParam &p) { D__rbin(f_rmod<S>,p); }

BL VecOp::d_min(OpParam &p) { D__rbin(f_rmin<S>,p); }
BL VecOp::d_cmin(OpParam &p) { D__cbin(f_cmin<S>,p); }
BL VecOp::d_max(OpParam &p) { D__rbin(f_rmax<S>,p); }
BL VecOp::d_cmax(OpParam &p) { D__cbin(f_cmax<S>,p); }

BL VecOp::d_lwr(OpParam &p) { D__rbin(f_rlwr<S>,p); }
BL VecOp::d_gtr(OpParam &p) { D__rbin(f_rgtr<S>,p); }
BL VecOp::d_leq(OpParam &p) { D__rbin(f_rleq<S>,p); }
BL VecOp::d_geq(OpParam &p) { D__rbin(f_rgeq<S>,p); }
BL VecOp::d_equ(OpParam &p) { D__rbin(f_requ<S>,p); }
BL VecOp::d_neq(OpParam &p) { D__rbin(f_rneq<S>,p); }

BL VecOp::d_pow(OpParam &p) { D__rbin(f_rpow<S>,p); }

BL VecOp::d_sqr(OpParam &p) { D__run(f_rsqr<S>,p); }
BL VecOp::d_ssqr(OpParam &p) { D__run(f_rssqr<S>,p); }
BL VecOp::d_csqr(OpParam &p) { D__cun(f_csqr<S>,p); }
BL VecOp::d_sqrt(OpParam &p) { D__run(f_rsqrt<S>,p); }
BL VecOp::d_ssqrt(OpParam &p) { D__run(f_rssqrt<S>,p); }

BL VecOp::d_exp(OpParam &p) { D__run(f_rexp<S>,p); }
BL VecOp::d_log(OpParam &p) { D__run(f_rlog<S>,p); }

BL VecOp::d_inv(OpParam &p) { D__run(f_rinv<S>,p); }
BL VecOp::d_cinv(OpParam &p) { D__cun(f_cinv<S>,p); }

BL VecOp::d_cnorm(OpParam &p) { D__cun(f_cnorm<S>,p); }

BL VecOp::d_abs(OpParam &p) { D__run(f_rabs<S>,p); }
BL VecOp::d_sign(OpParam &p) { D__run(f_rsign<S>,p); }

BL VecOp::d_polar(OpParam &p) { D__cun(f_polar<S>,p); }
BL VecOp::d_cart(OpParam &p) { D__cun(f_cart<S>,p); }
BL VecOp::d_cswap(OpParam &p) { D__cun(f_cswap<S>,p); }
BL VecOp::d_cconj(OpParam &p) { D__cun(f_cconj<S>,p); }

BL VecOp::d_minmax(OpParam &p) { D__cun(f_minmax<S>,p); }







#if 0

TEMPL1(STR)
static BL d_scopy(I cnt,F *dst,I _str,F val) 
{ 
	TINIT1(str,STR);
	for(I i = 0; i < cnt; ++i,dst += str) *dst = val; 
	return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_copy(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst = *src; 
	return true; 
}

TEMPL2(RSTR,ISTR)
static BL d_ccopy(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	TINIT2(rstr,RSTR,istr,ISTR);
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst = re,*idst = im; 
	return true; 
}

static BL d_vcopy(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	// TODO: check for src/dst-overlap!
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) 
			*rdst = *rsrc,*idst = *isrc; 
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr,idst += distr) 
			*rdst = *rsrc,*idst = 0.; 
	return true;
}

static const Vasp::arg_funcs f_copy = { TF1(d_scopy),TF2(d_ccopy),TF2(d_copy),d_vcopy };

Vasp *Vasp::m_copy(const Argument &arg) { return fr_arg("copy",arg,f_copy); }
Vasp *Vasp::m_ccopy(const Argument &arg) { return fc_arg("ccopy",arg,f_copy); }
//Vasp *Vasp::m_vcopy(const Argument &arg) { return fv_arg("vcopy",arg,f_copy); }


TEMPL1(STR)
static BL d_add(I cnt,F *dst,I _str,F v) 
{ 
	TINIT1(str,STR);
	for(I i = 0; i < cnt; ++i,dst += str) *dst += v; 
	return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_add(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst += *src; 
	return true; 
}

TEMPL2(RSTR,ISTR)
static BL d_add(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	TINIT2(rstr,RSTR,istr,ISTR);
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst += re,*idst += im; 
	return true; 
}

static BL d_add(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	// TODO: check for src/dst-overlap!
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) 
			*rdst += *rsrc,*idst += *isrc; 
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr) 
			*rdst += *rsrc; 
	return true; 
}

static const Vasp::arg_funcs f_add = { TF1(d_add),TF2(d_add),TF2(d_add),d_add };

Vasp *Vasp::m_add(const Argument &arg) { return fr_arg("add",arg,f_add); }
Vasp *Vasp::m_cadd(const Argument &arg) { return fc_arg("cadd",arg,f_add); }
//Vasp *Vasp::m_vadd(const Argument &arg) { return fv_arg("vadd",arg,f_add); }


TEMPL1(STR)
static BL d_sub(I cnt,F *dst,I _str,F v) 
{ 
	TINIT1(str,STR);
	for(I i = 0; i < cnt; ++i,dst += str) *dst -= v; 
	return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_sub(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst -= *src; 
	return true; 
}

TEMPL2(RSTR,ISTR)
static BL d_sub(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	TINIT2(rstr,RSTR,istr,ISTR);
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst -= re,*idst -= im; 
	return true; 
}

static BL d_sub(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	// TODO: check for src/dst-overlap!
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) 
			*rdst -= *rsrc,*idst -= *isrc; 
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr) 
			*rdst -= *rsrc; 
	return true; 
}

static const Vasp::arg_funcs f_sub = { TF1(d_sub),TF2(d_sub),TF2(d_sub),d_sub };

Vasp *Vasp::m_sub(const Argument &arg) { return fr_arg("sub",arg,f_sub); }
Vasp *Vasp::m_csub(const Argument &arg) { return fc_arg("csub",arg,f_sub); }
//Vasp *Vasp::m_vsub(const Argument &arg) { return fv_arg("vsub",arg,f_sub); }


TEMPL1(STR)
static BL d_mul(I cnt,F *dst,I _str,F v) 
{ 
	TINIT1(str,STR);
	for(I i = 0; i < cnt; ++i,dst += str) *dst *= v; 
	return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_mul(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) *dst *= *src; 
	return true; 
}

TEMPL2(RSTR,ISTR)
static BL d_mul(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	TINIT2(rstr,RSTR,istr,ISTR);
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) {
		register const F r = *rdst *re-*idst *im;
		*idst = *idst *re+*rdst *im;
		*rdst = r;
	}
	return true; 
}

static BL d_mul(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	// TODO: check for src/dst-overlap!
	if(isrc) 
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) {
			register const F r = *rdst * *rsrc-*idst * *isrc;
			*idst = *idst * *rsrc+*rdst * *isrc;
			*rdst = r;
		}
	else 
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr,idst += distr) 
			*rdst *= *rsrc,*idst *= *rsrc;
	return true; 
}

static const Vasp::arg_funcs f_mul = { TF1(d_mul),TF2(d_mul),TF2(d_mul),d_mul };

Vasp *Vasp::m_mul(const Argument &arg) { return fr_arg("mul",arg,f_mul); }
Vasp *Vasp::m_cmul(const Argument &arg) { return fc_arg("cmul",arg,f_mul); }
//Vasp *Vasp::m_vmul(const Argument &arg) { return fv_arg("vmul",arg,f_mul); }


// how about div by 0?

TEMPL1(STR)
static BL d_div(I cnt,F *dst,I _str,F v) 
{ 
	TINIT1(str,STR);
	register const F iv = 1./v; 
	for(I i = 0; i < cnt; ++i,dst += str) *dst *= iv; return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_div(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst /= *src; 
	return true; 
}

TEMPL2(RSTR,ISTR)
static BL d_div(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	TINIT2(rstr,RSTR,istr,ISTR);
	F den = abs(re,im);
	register F re1 = re/den,im1 = im/den;
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) {
		register const F r = *rdst *re1+*idst *im1;
		*idst = *idst *re1-*rdst *im1;
		*rdst = r;
	}
	return true; 
}

static BL d_div(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	// TODO: check for src/dst-overlap!
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) {
			register const F den = abs(*rsrc,*isrc);
			register const F re1 = *rsrc/den,im1 = *isrc/den;
			register const F r = *rdst * re1+*idst * im1;
			*idst = *idst * re1-*rdst * im1;
			*rdst = r;
		}
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr,idst += distr) {
			register F den = 1./ *rsrc;
			*rdst *= den,*idst *= den;
		}	
	return true; 
}

static const Vasp::arg_funcs f_div = { TF1(d_div),TF2(d_div),TF2(d_div),d_div };

Vasp *Vasp::m_div(const Argument &arg) { return fr_arg("div",arg,f_div); }
Vasp *Vasp::m_cdiv(const Argument &arg) { return fc_arg("cdiv",arg,f_div); }
//Vasp *Vasp::m_vdiv(const Argument &arg) { return fv_arg("vdiv",arg,f_div); }




TEMPL1(STR)
static BL d_min(I cnt,F *dst,I _str,F v) 
{ 
	TINIT1(str,STR);
	register const F va = fabs(v);
	for(I i = 0; i < cnt; ++i,dst += str) if(fabs(*dst) > v) *dst = v;
	return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_min(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) if(fabs(*dst) > fabs(*src)) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_min = { TF1(d_min),NULL,TF2(d_min),NULL };

Vasp *Vasp::m_min(const Argument &arg) { return fr_arg("min",arg,f_min); }
//Vasp *Vasp::m_cmin(const Argument &arg) { return fc_arg(argc,arg,d_min); }
//Vasp *Vasp::m_vmin(const Argument &arg) { return fv_arg("vmin",arg,f_min); }



TEMPL1(STR)
static BL d_max(I cnt,F *dst,I _str,F v) 
{ 
	TINIT1(str,STR);
	register const F va = fabs(v);
	for(I i = 0; i < cnt; ++i,dst += str) if(fabs(*dst) < va) *dst = v;
	return true; 
}

TEMPL2(DSTR,SSTR)
static BL d_max(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	TINIT2(dstr,DSTR,sstr,SSTR);
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) if(fabs(*dst) < fabs(*src)) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_max = { TF1(d_max),NULL,TF2(d_max),NULL };

Vasp *Vasp::m_max(const Argument &arg) { return fr_arg("max",arg,f_max); }
//Vasp *Vasp::m_cmax(const Argument &arg) { return fc_arg(arg,d_max,d_max); }
//Vasp *Vasp::m_vmax(const Argument &arg) { return fv_arg("vmax",arg,f_max); }


TEMPL2(RSTR,ISTR)
static BL d_minmax(I cnt,F *re,I _rstr,F *im,I _istr,F,F) 
{ 
	TINIT2(rstr,RSTR,istr,ISTR);
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) if(fabs(*re) > fabs(*im)) swap(*re,*im);
	return true; 
}

Vasp *Vasp::m_minmax() { return fc_arg("minmax",CX(),TF2(d_minmax)); }

#endif
