/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


template <I STR>
static BL d_copy(I cnt,F *dst,I _str,F val) 
{ 
	const I str = STR?STR:_str;
	for(I i = 0; i < cnt; ++i,dst += str) *dst = val; 
	return true; 
}

template <I DSTR,I SSTR>
static BL d_copy(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst = *src; 
	return true; 
}

template <I RSTR,I ISTR>
static BL d_copy(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	const I rstr = RSTR?RSTR:_rstr,istr = ISTR?ISTR:_istr;
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst = re,*idst = im; 
	return true; 
}

static BL d_copy(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
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

static const Vasp::arg_funcs f_copy = { d_copy<0>,d_copy<0,0>,d_copy<0,0>,d_copy };

Vasp *Vasp::m_copy(const Argument &arg) { return fr_arg("copy",arg,f_copy); }
Vasp *Vasp::m_ccopy(const Argument &arg) { return fc_arg("ccopy",arg,f_copy); }
Vasp *Vasp::m_vcopy(const Argument &arg) { return fv_arg("vcopy",arg,f_copy); }


template <I STR>
static BL d_add(I cnt,F *dst,I _str,F v) 
{ 
	const I str = STR?STR:_str;
	for(I i = 0; i < cnt; ++i,dst += str) *dst += v; 
	return true; 
}

template <I DSTR,I SSTR>
static BL d_add(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst += *src; 
	return true; 
}

template <I RSTR,I ISTR>
static BL d_add(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	const I rstr = RSTR?RSTR:_rstr,istr = ISTR?ISTR:_istr;
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

static const Vasp::arg_funcs f_add = { d_add<0>,d_add<0,0>,d_add<0,0>,d_add };

Vasp *Vasp::m_add(const Argument &arg) { return fr_arg("add",arg,f_add); }
Vasp *Vasp::m_cadd(const Argument &arg) { return fc_arg("cadd",arg,f_add); }
Vasp *Vasp::m_vadd(const Argument &arg) { return fv_arg("vadd",arg,f_add); }

template <I STR>
static BL d_sub(I cnt,F *dst,I _str,F v) 
{ 
	const I str = STR?STR:_str;
	for(I i = 0; i < cnt; ++i,dst += str) *dst -= v; 
	return true; 
}

template <I DSTR,I SSTR>
static BL d_sub(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst -= *src; 
	return true; 
}

template <I RSTR,I ISTR>
static BL d_sub(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	const I rstr = RSTR?RSTR:_rstr,istr = ISTR?ISTR:_istr;
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

static const Vasp::arg_funcs f_sub = { d_sub<0>,d_sub<0,0>,d_sub<0,0>,d_sub };

Vasp *Vasp::m_sub(const Argument &arg) { return fr_arg("sub",arg,f_sub); }
Vasp *Vasp::m_csub(const Argument &arg) { return fc_arg("csub",arg,f_sub); }
Vasp *Vasp::m_vsub(const Argument &arg) { return fv_arg("vsub",arg,f_sub); }


template <I STR>
static BL d_mul(I cnt,F *dst,I _str,F v) 
{ 
	const I str = STR?STR:_str;
	for(I i = 0; i < cnt; ++i,dst += str) *dst *= v; 
	return true; 
}

template <I DSTR,I SSTR>
static BL d_mul(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) *dst *= *src; 
	return true; 
}

template <I RSTR,I ISTR>
static BL d_mul(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	const I rstr = RSTR?RSTR:_rstr,istr = ISTR?ISTR:_istr;
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) {
		register F r = *rdst *re-*idst *im;
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
			register F r = *rdst * *rsrc-*idst * *isrc;
			*idst = *idst * *rsrc+*rdst * *isrc;
			*rdst = r;
		}
	else 
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr,idst += distr) 
			*rdst *= *rsrc,*idst *= *rsrc;
	return true; 
}

static const Vasp::arg_funcs f_mul = { d_mul<0>,d_mul<0,0>,d_mul<0,0>,d_mul };

Vasp *Vasp::m_mul(const Argument &arg) { return fr_arg("mul",arg,f_mul); }
Vasp *Vasp::m_cmul(const Argument &arg) { return fc_arg("cmul",arg,f_mul); }
Vasp *Vasp::m_vmul(const Argument &arg) { return fv_arg("vmul",arg,f_mul); }


// how about div by 0?

template <I STR>
static BL d_div(I cnt,F *dst,I _str,F v) 
{ 
	const I str = STR?STR:_str;
	F iv = 1./v; 
	for(I i = 0; i < cnt; ++i,dst += str) *dst *= iv; return true; 
}

template <I DSTR,I SSTR>
static BL d_div(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	// TODO: check for src/dst-overlap!
	for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst /= *src; 
	return true; 
}

template <I RSTR,I ISTR>
static BL d_div(I cnt,F *rdst,I _rstr,F *idst,I _istr,F re,F im) 
{ 
	const I rstr = RSTR?RSTR:_rstr,istr = ISTR?ISTR:_istr;
	F den = abs(re,im);
	register F re1 = re/den,im1 = im/den;
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) {
		register F r = *rdst *re1+*idst *im1;
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
			register F den = abs(*rsrc,*isrc);
			register F re1 = *rsrc/den,im1 = *isrc/den;
			register F r = *rdst * re1+*idst * im1;
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

static const Vasp::arg_funcs f_div = { d_div<0>,d_div<0,0>,d_div<0,0>,d_div };

Vasp *Vasp::m_div(const Argument &arg) { return fr_arg("div",arg,f_div); }
Vasp *Vasp::m_cdiv(const Argument &arg) { return fc_arg("cdiv",arg,f_div); }
Vasp *Vasp::m_vdiv(const Argument &arg) { return fv_arg("vdiv",arg,f_div); }




template <I STR>
static BL d_min(I cnt,F *dst,I _str,F v) 
{ 
	const I str = STR?STR:_str;
	for(I i = 0; i < cnt; ++i,dst += str) if(*dst > v) *dst = v;
	return true; 
}

template <I DSTR,I SSTR>
static BL d_min(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) if(*dst > *src) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_min = { d_min<0>,NULL,d_min<0,0>,NULL };

Vasp *Vasp::m_min(const Argument &arg) { return fr_arg("min",arg,f_min); }
//Vasp *Vasp::m_cmin(const Argument &arg) { return fc_arg(argc,arg,d_min); }
Vasp *Vasp::m_vmin(const Argument &arg) { return fv_arg("vmin",arg,f_min); }



template <I STR>
static BL d_max(I cnt,F *dst,I _str,F v) 
{ 
	const I str = STR?STR:_str;
	for(I i = 0; i < cnt; ++i,dst += str) if(*dst < v) *dst = v;
	return true; 
}

template <I DSTR,I SSTR>
static BL d_max(I cnt,F *dst,I _dstr,const F *src,I _sstr) 
{ 
	const I dstr = DSTR?DSTR:_dstr,sstr = SSTR?SSTR:_sstr;
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) if(*dst < *src) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_max = { d_max<0>,NULL,d_max<0,0>,NULL };

Vasp *Vasp::m_max(const Argument &arg) { return fr_arg("max",arg,f_max); }
//Vasp *Vasp::m_cmax(const Argument &arg) { return fc_arg(arg,d_max,d_max); }
Vasp *Vasp::m_vmax(const Argument &arg) { return fv_arg("vmax",arg,f_max); }

