/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

#ifdef _MSC_VER

#define TEMPL1(V1) 
#define TINIT1(L1,V1) const I L1 = _##L1
#define TF1(FUN) FUN

#define TEMPL2(V1,V2) 
#define TINIT2(L1,V1,L2,V2) const I L1 = _##L1,L2 = _##L2
#define TF2(FUN) FUN

#else

// template optimization on
#define TEMPL1(V1) template <I V1>
#define TINIT1(L1,V1) const I L1 = V1?V1:_##L1
#define TF1(FUN) FUN<0>

#define TEMPL2(V1,V2) template <I V1,I V2>
#define TINIT2(L1,V1,L2,V2) const I L1 = V1?V1:_##L1,L2 = V2?V2:_##L2
#define TF2(FUN) FUN<0,0>

#endif

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
