/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

#define PI 3.1415926535897932385

inline F sgn(F x) { return x < 0.?-1.F:1.F; }


static BL d_copy(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] = val; return true; }
static BL d_copy(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] = src[i]; return true; }
static BL d_copy(F *rdst,F *idst,F re,F im,I cnt) { for(I i = 0; i < cnt; ++i) rdst[i] = re,idst[i] = im; return true; }

static BL d_copy(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) rdst[i] = rsrc[i],idst[i] = isrc[i]; 
	else
		for(I i = 0; i < cnt; ++i) rdst[i] = rsrc[i],idst[i] = 0.; 
	return true;
}

static const Vasp::arg_funcs f_copy = { d_copy,d_copy,d_copy,d_copy };

Vasp *Vasp::m_copy(const Argument &arg) { return fr_arg("copy",arg,f_copy); }
Vasp *Vasp::m_ccopy(const Argument &arg) { return fc_arg("ccopy",arg,f_copy); }
Vasp *Vasp::m_mcopy(const Argument &arg) { return fm_arg("mcopy",arg,f_copy); }


static BL d_add(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] += v; return true; }
static BL d_add(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] += src[i]; return true; }
static BL d_add(F *rdst,F *idst,F re,F im,I cnt) { for(I i = 0; i < cnt; ++i) rdst[i] += re,idst[i] += im; return true; }

static BL d_add(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) rdst[i] += rsrc[i],idst[i] += isrc[i]; 
	else
		for(I i = 0; i < cnt; ++i) rdst[i] += rsrc[i]; 
	return true; 
}

static const Vasp::arg_funcs f_add = { d_add,d_add,d_add,d_add };

Vasp *Vasp::m_add(const Argument &arg) { return fr_arg("add",arg,f_add); }
Vasp *Vasp::m_cadd(const Argument &arg) { return fc_arg("cadd",arg,f_add); }
Vasp *Vasp::m_madd(const Argument &arg) { return fm_arg("madd",arg,f_add); }


static BL d_sub(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] -= v; return true; }
static BL d_sub(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] -= src[i]; return true; }
static BL d_sub(F *rdst,F *idst,F re,F im,I cnt) { for(I i = 0; i < cnt; ++i) rdst[i] -= re,idst[i] -= im; return true; }

static BL d_sub(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) rdst[i] -= rsrc[i],idst[i] -= isrc[i]; 
	else
		for(I i = 0; i < cnt; ++i) rdst[i] -= rsrc[i]; 
	return true; 
}

static const Vasp::arg_funcs f_sub = { d_sub,d_sub,d_sub,d_sub };

Vasp *Vasp::m_sub(const Argument &arg) { return fr_arg("sub",arg,f_sub); }
Vasp *Vasp::m_csub(const Argument &arg) { return fc_arg("csub",arg,f_sub); }
Vasp *Vasp::m_msub(const Argument &arg) { return fm_arg("msub",arg,f_sub); }


static BL d_mul(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= v; return true; }
static BL d_mul(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= src[i]; return true; }

static BL d_mul(F *rdst,F *idst,F re,F im,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) {
		register F r = rdst[i]*re-idst[i]*im;
		idst[i] = idst[i]*re+rdst[i]*im;
		rdst[i] = r;
	}
	return true; 
}

static BL d_mul(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc) 
		for(I i = 0; i < cnt; ++i) {
			register F r = rdst[i]*rsrc[i]-idst[i]*isrc[i];
			idst[i] = idst[i]*rsrc[i]+rdst[i]*isrc[i];
			rdst[i] = r;
		}
	else 
		for(I i = 0; i < cnt; ++i) 
			rdst[i] *= rsrc[i],idst[i] *= rsrc[i];
	return true; 
}

static const Vasp::arg_funcs f_mul = { d_mul,d_mul,d_mul,d_mul };

Vasp *Vasp::m_mul(const Argument &arg) { return fr_arg("mul",arg,f_mul); }
Vasp *Vasp::m_cmul(const Argument &arg) { return fc_arg("cmul",arg,f_mul); }
Vasp *Vasp::m_mmul(const Argument &arg) { return fm_arg("mmul",arg,f_mul); }


// how about div by 0?

static BL d_div(F *dst,F v,I cnt) { F iv = 1./v; for(I i = 0; i < cnt; ++i) dst[i] *= iv; return true; }
static BL d_div(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] /= src[i]; return true; }

static BL d_div(F *rdst,F *idst,F re,F im,I cnt) 
{ 
	F den = re*re+im*im;
	register F re1 = re/den,im1 = im/den;
	for(I i = 0; i < cnt; ++i) {
		register F r = rdst[i]*re1+idst[i]*im1;
		idst[i] = idst[i]*re1-rdst[i]*im1;
		rdst[i] = r;
	}
	return true; 
}

static BL d_div(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) {
			register F den = rsrc[i]*rsrc[i]+isrc[i]*isrc[i];
			register F re1 = rsrc[i]/den,im1 = isrc[i]/den;
			register F r = rdst[i]*re1+idst[i]*im1;
			idst[i] = idst[i]*re1-rdst[i]*im1;
			rdst[i] = r;
		}
	else
		for(I i = 0; i < cnt; ++i) rdst[i] /= rsrc[i],idst[i] /= rsrc[i];
	return true; 
}

static const Vasp::arg_funcs f_div = { d_div,d_div,d_div,d_div };

Vasp *Vasp::m_div(const Argument &arg) { return fr_arg("div",arg,f_div); }
Vasp *Vasp::m_cdiv(const Argument &arg) { return fc_arg("cdiv",arg,f_div); }
Vasp *Vasp::m_mdiv(const Argument &arg) { return fm_arg("mdiv",arg,f_div); }




static BL d_min(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst > val) *dst = val;
	return true; 
}

static BL d_min(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst > *src) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_min = { d_min,NULL,d_min,NULL };

Vasp *Vasp::m_min(const Argument &arg) { return fr_arg("min",arg,f_min); }
//Vasp *Vasp::m_cmin(const Argument &arg) { return fc_arg(argc,arg,d_min); }
Vasp *Vasp::m_mmin(const Argument &arg) { return fm_arg("mmin",arg,f_min); }



static BL d_max(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst < val) *dst = val;
	return true; 
}

static BL d_max(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst < *src) *dst = *src;
	return true; 
}

static const Vasp::arg_funcs f_max = { d_max,NULL,d_max,NULL };

Vasp *Vasp::m_max(const Argument &arg) { return fr_arg("max",arg,f_max); }
//Vasp *Vasp::m_cmax(const Argument &arg) { return fc_arg(arg,d_max,d_max); }
Vasp *Vasp::m_mmax(const Argument &arg) { return fm_arg("mmax",arg,f_max); }





static BL d_pow(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)pow(fabs(*dst),val)*sgn(*dst);
	return true; 
}

static BL d_sqr(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = *dst * *dst; return true; }
static BL d_ssqr(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)(*dst * fabs(*dst)); return true; }

static BL d_csqr(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		F r = *re * *re - *im * *im;
		*im *= *re * 2;
		*re = r;
	}
	return true; 
}

static BL d_root(F *dst,F val,I cnt) 
{ 
	if(val == 0) return true;
	F rad = 1.F/val;
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)pow(fabs(*dst),rad)*sgn(*dst);
	return true; 
}

static BL d_sqrt(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)sqrt(fabs(*dst));
	return true; 
}

static BL d_ssqrt(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)sqrt(fabs(*dst))*sgn(*dst);
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


static BL d_exp(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)exp(*dst); return true; }

// how about numbers <= 0?
static BL d_log(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)log(*dst); return true; }

Vasp *Vasp::m_exp() { return fr_nop("exp",0,d_exp); }
Vasp *Vasp::m_log() { return fr_nop("log",0,d_log); }


static BL d_inv(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = 1./ *dst; return true; }

static BL d_cinv(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		register F den = *re * *re+*im * *im;
		*re /= den,*im /= -den;
	}
	return true; 
}

Vasp *Vasp::m_inv() { return fr_nop("inv",0,d_inv); }
Vasp *Vasp::m_cinv() { return fc_nop("cinv",CX(),d_cinv); }



static BL d_abs(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = fabs(*dst);
	return true; 
}

static BL d_sign(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = *dst == 0?0:(*dst < 0?-1.F:1.F);
	return true; 
}

static BL d_polar(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		register F abs = *re * *re+*im * *im;
		if(*re)
			*im = atan(*im / *re);
		else
			if(abs) *im = *im > 0?PI/2:-PI/2;
		*re = abs;
	}
	return true; 
}

static BL d_cart(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
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





static BL d_norm(F *dst,F,I cnt) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i) { F s = fabs(dst[i]); if(s > v) v = s; }
	v = 1./v;
	for(i = 0; i < cnt; ++i) dst[i] *= v;
	return true; 
}

static BL d_cnorm(F *re,F *im,F,F,I cnt) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i) { 
		register F abs = re[i]*re[i]+im[i]*im[i]; 
		if(abs > v) v = abs; 
	}
	v = 1./sqrt(v);
	for(i = 0; i < cnt; ++i) re[i] *= v,im[i] *= v;
	return true; 
}

Vasp *Vasp::m_norm() { return fr_nop("norm",0,d_norm); }
Vasp *Vasp::m_cnorm() { return fc_nop("cnorm",CX(),d_cnorm); }



static BL d_cswap(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) { register F r = *re; *im = *re,*re = r; }
	return true; 
}

static BL d_cconj(F *,F *im,F,F,I cnt) { for(I i = 0; i < cnt; ++i) *im *= -1.; return true; }

Vasp *Vasp::m_cswap() { return fc_nop("cswap",CX(),d_cswap); }
Vasp *Vasp::m_cconj() { return fc_nop("cconj",CX(),d_cconj); }




