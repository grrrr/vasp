/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932385
#endif

inline F sgn(F x) { return x < 0.?-1.F:1.F; }


static BL d_copy(I cnt,F *dst,I str,F val) 
{ for(I i = 0; i < cnt; ++i,dst += str) *dst = val; return true; }

static BL d_copy(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst = *src; return true; }

static BL d_copy(I cnt,F *rdst,I rstr,F *idst,I istr,F re,F im) 
{ 
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst = re,*idst = im; 
	return true; 
}

static BL d_copy(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) 
			*rdst = *rsrc,*idst = *isrc; 
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr,idst += distr) 
			*rdst = *rsrc,*idst = 0.; 
	return true;
}

static const Vasp::arg_funcs f_copy = { d_copy,d_copy,d_copy,d_copy };

Vasp *Vasp::m_copy(const Argument &arg) { return fr_arg("copy",arg,f_copy); }
Vasp *Vasp::m_ccopy(const Argument &arg) { return fc_arg("ccopy",arg,f_copy); }
Vasp *Vasp::m_mcopy(const Argument &arg) { return fm_arg("mcopy",arg,f_copy); }


static BL d_add(I cnt,F *dst,I str,F v) 
{ for(I i = 0; i < cnt; ++i,dst += str) *dst += v; return true; }

static BL d_add(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst += *src; return true; }

static BL d_add(I cnt,F *rdst,I rstr,F *idst,I istr,F re,F im) 
{ 
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst += re,*idst += im; 
	return true; 
}

static BL d_add(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) 
			*rdst += *rsrc,*idst += *isrc; 
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr) 
			*rdst += *rsrc; 
	return true; 
}

static const Vasp::arg_funcs f_add = { d_add,d_add,d_add,d_add };

Vasp *Vasp::m_add(const Argument &arg) { return fr_arg("add",arg,f_add); }
Vasp *Vasp::m_cadd(const Argument &arg) { return fc_arg("cadd",arg,f_add); }
Vasp *Vasp::m_madd(const Argument &arg) { return fm_arg("madd",arg,f_add); }

static BL d_sub(I cnt,F *dst,I str,F v) 
{ for(I i = 0; i < cnt; ++i,dst += str) *dst -= v; return true; }

static BL d_sub(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst -= *src; return true; }

static BL d_sub(I cnt,F *rdst,I rstr,F *idst,I istr,F re,F im) 
{ 
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) *rdst -= re,*idst -= im; 
	return true; 
}

static BL d_sub(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) 
			*rdst -= *rsrc,*idst -= *isrc; 
	else
		for(I i = 0; i < cnt; ++i,rsrc += srstr,rdst += drstr) 
			*rdst -= *rsrc; 
	return true; 
}

static const Vasp::arg_funcs f_sub = { d_sub,d_sub,d_sub,d_sub };

Vasp *Vasp::m_sub(const Argument &arg) { return fr_arg("sub",arg,f_sub); }
Vasp *Vasp::m_csub(const Argument &arg) { return fc_arg("csub",arg,f_sub); }
Vasp *Vasp::m_msub(const Argument &arg) { return fm_arg("msub",arg,f_sub); }


static BL d_mul(I cnt,F *dst,I str,F v) 
{ for(I i = 0; i < cnt; ++i,dst += str) *dst *= v; return true; }

static BL d_mul(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) *dst *= *src; return true; }

static BL d_mul(I cnt,F *rdst,I rstr,F *idst,I istr,F re,F im) 
{ 
	for(I i = 0; i < cnt; ++i,rdst += rstr,idst += istr) {
		register F r = *rdst *re-*idst *im;
		*idst = *idst *re+*rdst *im;
		*rdst = r;
	}
	return true; 
}

static BL d_mul(I cnt,F *rdst,I drstr,F *idst,I distr,const F *rsrc,I srstr,const F *isrc,I sistr) 
{ 
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

static const Vasp::arg_funcs f_mul = { d_mul,d_mul,d_mul,d_mul };

Vasp *Vasp::m_mul(const Argument &arg) { return fr_arg("mul",arg,f_mul); }
Vasp *Vasp::m_cmul(const Argument &arg) { return fc_arg("cmul",arg,f_mul); }
Vasp *Vasp::m_mmul(const Argument &arg) { return fm_arg("mmul",arg,f_mul); }


// how about div by 0?

static BL d_div(I cnt,F *dst,I str,F v) 
{ F iv = 1./v; for(I i = 0; i < cnt; ++i,dst += str) *dst *= iv; return true; }

static BL d_div(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ for(I i = 0; i < cnt; ++i,src += sstr,dst += dstr) *dst /= *src; return true; }

static BL d_div(I cnt,F *rdst,I rstr,F *idst,I istr,F re,F im) 
{ 
	F den = re*re+im*im;
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
	if(isrc)
		for(I i = 0; i < cnt; ++i,rsrc += srstr,isrc += sistr,rdst += drstr,idst += distr) {
			register F den = *rsrc * *rsrc+*isrc * *isrc;
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

static const Vasp::arg_funcs f_div = { d_div,d_div,d_div,d_div };

Vasp *Vasp::m_div(const Argument &arg) { return fr_arg("div",arg,f_div); }
Vasp *Vasp::m_cdiv(const Argument &arg) { return fc_arg("cdiv",arg,f_div); }
Vasp *Vasp::m_mdiv(const Argument &arg) { return fm_arg("mdiv",arg,f_div); }




static BL d_min(I cnt,F *dst,I str,F v) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) if(*dst > v) *dst = v;
	return true; 
}

static BL d_min(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ 
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) if(*dst > *src) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_min = { d_min,NULL,d_min,NULL };

Vasp *Vasp::m_min(const Argument &arg) { return fr_arg("min",arg,f_min); }
//Vasp *Vasp::m_cmin(const Argument &arg) { return fc_arg(argc,arg,d_min); }
Vasp *Vasp::m_mmin(const Argument &arg) { return fm_arg("mmin",arg,f_min); }



static BL d_max(I cnt,F *dst,I str,F v) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) if(*dst < v) *dst = v;
	return true; 
}

static BL d_max(I cnt,F *dst,I dstr,const F *src,I sstr) 
{ 
	for(I i = 0; i < cnt; ++i,dst += dstr,src += sstr) if(*dst < *src) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_max = { d_max,NULL,d_max,NULL };

Vasp *Vasp::m_max(const Argument &arg) { return fr_arg("max",arg,f_max); }
//Vasp *Vasp::m_cmax(const Argument &arg) { return fc_arg(arg,d_max,d_max); }
Vasp *Vasp::m_mmax(const Argument &arg) { return fm_arg("mmax",arg,f_max); }





static BL d_pow(I cnt,F *dst,I str,F v) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)pow(fabs(*dst),v)*sgn(*dst);
	return true; 
}

static BL d_sqr(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = *dst * *dst; return true; }
static BL d_ssqr(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)(*dst * fabs(*dst)); return true; }

static BL d_csqr(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		F r = *re * *re - *im * *im;
		*im *= *re * 2;
		*re = r;
	}
	return true; 
}

static BL d_root(I cnt,F *dst,I str,F v) 
{ 
	if(v == 0) return true;
	F rad = 1.F/v;
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)pow(fabs(*dst),rad)*sgn(*dst);
	return true; 
}

static BL d_sqrt(I cnt,F *dst,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)sqrt(fabs(*dst));
	return true; 
}

static BL d_ssqrt(I cnt,F *dst,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)sqrt(fabs(*dst))*sgn(*dst);
	return true; 
}

Vasp *Vasp::m_pow(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_nop("pow",arg.GetFloat(),d_pow):NULL; 
}

Vasp *Vasp::m_root(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_nop("root",arg.GetFloat(),d_root):NULL; 
}

//Vasp *Vasp::m_cpow(I argc,t_atom *argv) { return fm_arg("cpow",argc,argv,d_max); }
Vasp *Vasp::m_sqr() { return fr_nop("sqr",0,d_sqr); }
Vasp *Vasp::m_ssqr() { return fr_nop("ssqr",0,d_ssqr); }
Vasp *Vasp::m_csqr() { return fc_nop("csqr",CX(),d_csqr); }
Vasp *Vasp::m_sqrt() { return fr_nop("sqrt",0,d_sqrt); }
Vasp *Vasp::m_ssqrt() { return fr_nop("ssqrt",0,d_ssqrt); }


static BL d_exp(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)exp(*dst); return true; }

// how about numbers <= 0?
static BL d_log(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)log(*dst); return true; }

Vasp *Vasp::m_exp() { return fr_nop("exp",0,d_exp); }
Vasp *Vasp::m_log() { return fr_nop("log",0,d_log); }


static BL d_inv(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = 1./ *dst; return true; }

static BL d_cinv(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F den = *re * *re+*im * *im;
		*re /= den,*im /= -den;
	}
	return true; 
}

Vasp *Vasp::m_inv() { return fr_nop("inv",0,d_inv); }
Vasp *Vasp::m_cinv() { return fc_nop("cinv",CX(),d_cinv); }



static BL d_abs(I cnt,F *dst,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = fabs(*dst);
	return true; 
}

static BL d_sign(I cnt,F *dst,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = *dst == 0?0:(*dst < 0?-1.F:1.F);
	return true; 
}

static BL d_polar(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F abs = *re * *re+*im * *im;
		if(*re) 
			*im = atan(*im / *re)+(*re < 0?PI:0);
		else
			if(abs) *im = *im > 0?PI/2:-PI/2;
			else *im = 0;
		*re = abs;
	}
	return true; 
}

static BL d_cart(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F arg = *im;
		*im = *re*sin(arg);
		*re *= cos(arg);
	}
	return true; 
}

Vasp *Vasp::m_abs() { return fr_nop("inv",0,d_inv); }
Vasp *Vasp::m_sign() { return fr_nop("sign",0,d_sign); }
Vasp *Vasp::m_polar() { return fc_nop("polar",CX(),d_polar); }
Vasp *Vasp::m_cart() { return fc_nop("cart",CX(),d_cart); }





static BL d_norm(I cnt,F *dst,I str,F) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i,dst += str) { F s = fabs(*dst); if(s > v) v = s; }
	v = 1./v;
	for(i = 0; i < cnt; ++i) *(dst -= str) *= v;
	return true; 
}

static BL d_cnorm(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i,re += rstr,im += istr) { 
		register F abs = *re * *re+*im * *im; 
		if(abs > v) v = abs; 
	}
	v = 1./sqrt(v);
	for(i = 0; i < cnt; ++i) *(re -= rstr) *= v,*(im -= istr) *= v;
	return true; 
}

Vasp *Vasp::m_norm() { return fr_nop("norm",0,d_norm); }
Vasp *Vasp::m_cnorm() { return fc_nop("cnorm",CX(),d_cnorm); }



static BL d_cswap(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) { register F r = *re; *im = *re,*re = r; }
	return true; 
}

static BL d_cconj(I cnt,F *,I,F *im,I istr,F,F) { for(I i = 0; i < cnt; ++i,im += istr) *im *= -1.; return true; }

Vasp *Vasp::m_cswap() { return fc_nop("cswap",CX(),d_cswap); }
Vasp *Vasp::m_cconj() { return fc_nop("cconj",CX(),d_cconj); }




