/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "vasp__op.h"


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


/*! \brief skeleton for unary real operations
	\todo optimization for src=dst
*/
template <V fun(S &v,S a)>
static BL d__run(OpParam &p) 
{ 
	for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds) 
		fun(*p.rddt,*p.rsdt); 
	return true; 
}

/*! \brief skeleton for unary complex operations
	\todo optimization for src=dst
*/
template <V fun(S &rv,S &iv,S ra,S ia)>
static BL d__cun(OpParam &p) 
{ 
	if(p.isdt)
		for(; p.frames--; p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids) 
			fun(*p.rddt,*p.iddt,*p.rsdt,*p.isdt);
	else
		for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds,p.iddt += p.ids) 
			fun(*p.rddt,*p.iddt,*p.rsdt,0);
	return true; 
}

/*! \brief skeleton for binary real operations
	\todo optimization for src=dst
*/
template <V fun(S &v,S a,S b)>
static BL d__rbin(OpParam &p) 
{ 
	if(p.HasArg()) {
		for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds,p.radt += p.ras) 
			fun(*p.rddt,*p.rsdt,*p.radt); 
	}
	else {
		S v = p.rbin.arg;
		for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds) 
			fun(*p.rddt,*p.rsdt,v); 
	}

	return true; 
}

/*! \brief skeleton for binary complex operations
	\todo optimization for src=dst
*/
template <V fun(S &rv,S &iv,S ra,S ia,S rb,S ib)>
static BL d__cbin(OpParam &p) 
{ 
	if(p.HasArg()) {
		if(p.isdt)
			if(p.iadt)
				for(; p.frames--; p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids,p.radt += p.ras,p.iadt += p.ias) 
					fun(*p.rddt,*p.iddt,*p.rsdt,*p.isdt,*p.radt,*p.iadt);
			else
				for(; p.frames--; p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids,p.radt += p.ras) 
					fun(*p.rddt,*p.iddt,*p.rsdt,*p.isdt,*p.radt,0);
		else
			if(p.iadt)
				for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds,p.iddt += p.ids,p.radt += p.ras,p.iadt += p.ias) 
					fun(*p.rddt,*p.iddt,*p.rsdt,0,*p.radt,*p.iadt);
			else
				for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds,p.iddt += p.ids,p.radt += p.ras) 
					fun(*p.rddt,*p.iddt,*p.rsdt,0,*p.radt,0);
	}
	else {
		S rv = p.cbin.rarg,iv = p.cbin.iarg;
		if(p.isdt)
			for(; p.frames--; p.rsdt += p.rss,p.isdt += p.iss,p.rddt += p.rds,p.iddt += p.ids) 
				fun(*p.rddt,*p.iddt,*p.rsdt,*p.isdt,rv,iv);
		else
			for(; p.frames--; p.rsdt += p.rss,p.rddt += p.rds,p.iddt += p.ids) 
				fun(*p.rddt,*p.iddt,*p.rsdt,0,rv,iv);
	}

	return true; 
}



// --------------------------------------------------------------

BL VecOp::d_copy(OpParam &p) { return d__rbin<f_rcopy>(p); }
BL VecOp::d_ccopy(OpParam &p) { return d__cbin<f_ccopy>(p); }
BL VecOp::d_add(OpParam &p) { return d__rbin<f_radd>(p); }
BL VecOp::d_cadd(OpParam &p) { return d__cbin<f_cadd>(p); }
BL VecOp::d_sub(OpParam &p) { return d__rbin<f_rsub>(p); }
BL VecOp::d_csub(OpParam &p) { return d__cbin<f_csub>(p); }
BL VecOp::d_mul(OpParam &p) { return d__rbin<f_rmul>(p); }
BL VecOp::d_cmul(OpParam &p) { return d__cbin<f_cmul>(p); }
BL VecOp::d_div(OpParam &p) { return d__rbin<f_rdiv>(p); }
BL VecOp::d_cdiv(OpParam &p) { return d__cbin<f_cdiv>(p); }

BL VecOp::d_min(OpParam &p) { return d__rbin<f_rmin>(p); }
BL VecOp::d_cmin(OpParam &p) { return d__cbin<f_cmin>(p); }
BL VecOp::d_max(OpParam &p) { return d__rbin<f_rmax>(p); }
BL VecOp::d_cmax(OpParam &p) { return d__cbin<f_cmax>(p); }

BL VecOp::d_minmax(OpParam &p) { return d__cbin<f_minmax>(p); }

BL VecOp::d_lwr(OpParam &p) { return d__rbin<f_rlwr>(p); }
BL VecOp::d_gtr(OpParam &p) { return d__rbin<f_rgtr>(p); }

BL VecOp::d_pow(OpParam &p) { return d__rbin<f_rpow>(p); }

BL VecOp::d_sqr(OpParam &p) { return d__run<f_rsqr>(p); }
BL VecOp::d_ssqr(OpParam &p) { return d__run<f_rssqr>(p); }
BL VecOp::d_csqr(OpParam &p) { return d__cun<f_csqr>(p); }
BL VecOp::d_sqrt(OpParam &p) { return d__run<f_rsqrt>(p); }
BL VecOp::d_ssqrt(OpParam &p) { return d__run<f_rssqrt>(p); }

BL VecOp::d_exp(OpParam &p) { return d__run<f_rexp>(p); }
BL VecOp::d_log(OpParam &p) { return d__run<f_rlog>(p); }

BL VecOp::d_inv(OpParam &p) { return d__run<f_rinv>(p); }
BL VecOp::d_cinv(OpParam &p) { return d__cun<f_cinv>(p); }

BL VecOp::d_cnorm(OpParam &p) { return d__cun<f_cnorm>(p); }

BL VecOp::d_abs(OpParam &p) { return d__run<f_rabs>(p); }
BL VecOp::d_sign(OpParam &p) { return d__run<f_rsign>(p); }

BL VecOp::d_polar(OpParam &p) { return d__cun<f_polar>(p); }
BL VecOp::d_cart(OpParam &p) { return d__cun<f_cart>(p); }
BL VecOp::d_cswap(OpParam &p) { return d__cun<f_cswap>(p); }
BL VecOp::d_cconj(OpParam &p) { return d__cun<f_cconj>(p); }







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
