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


// --- power functions ---------------------

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
	return arg.IsFloat()?fr_arg("pow",arg.GetFloat(),d_pow):NULL; 
}

Vasp *Vasp::m_root(const Argument &arg) 
{ 
	return arg.IsFloat()?fr_arg("root",arg.GetFloat(),d_root):NULL; 
}

//Vasp *Vasp::m_cpow(I argc,t_atom *argv) { return fm_arg("cpow",argc,argv,d_max); }
Vasp *Vasp::m_sqr() { return fr_arg("sqr",0,d_sqr); }
Vasp *Vasp::m_ssqr() { return fr_arg("ssqr",0,d_ssqr); }
Vasp *Vasp::m_csqr() { return fc_arg("csqr",CX(),d_csqr); }
Vasp *Vasp::m_sqrt() { return fr_arg("sqrt",0,d_sqrt); }
Vasp *Vasp::m_ssqrt() { return fr_arg("ssqrt",0,d_ssqrt); }


static BL d_exp(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)exp(*dst); return true; }

// how about numbers <= 0?
static BL d_log(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)log(*dst); return true; }

Vasp *Vasp::m_exp() { return fr_arg("exp",0,d_exp); }
Vasp *Vasp::m_log() { return fr_arg("log",0,d_log); }


static BL d_inv(I cnt,F *dst,I str,F) { for(I i = 0; i < cnt; ++i,dst += str) *dst = 1./ *dst; return true; }

static BL d_cinv(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F den = abs(*re,*im);
		*re /= den,*im /= -den;
	}
	return true; 
}

Vasp *Vasp::m_inv() { return fr_arg("inv",0,d_inv); }
Vasp *Vasp::m_cinv() { return fc_arg("cinv",CX(),d_cinv); }


// --- complex domain functions -----------------

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
		register F _abs = abs(*re,*im);
		*im = arg(*re,*im);
		*re = _abs;
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

Vasp *Vasp::m_abs() { return fr_arg("inv",0,d_inv); }
Vasp *Vasp::m_sign() { return fr_arg("sign",0,d_sign); }
Vasp *Vasp::m_polar() { return fc_arg("polar",CX(),d_polar); }
Vasp *Vasp::m_cart() { return fc_arg("cart",CX(),d_cart); }





static BL d_norm(I cnt,F *dst,I str,F) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i,dst += str) { F s = fabs(*dst); if(s > v) v = s; }

	if(v != 1) {
		v = 1./v;
		for(i = 0; i < cnt; ++i) *(dst -= str) *= v;
	}
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

	if(v != 1) {
		v = 1./sqrt(v);
		for(i = 0; i < cnt; ++i) *(re -= rstr) *= v,*(im -= istr) *= v;
	}
	return true; 
}

Vasp *Vasp::m_norm() { return fr_arg("norm",0,d_norm); }
Vasp *Vasp::m_cnorm() { return fc_arg("cnorm",CX(),d_cnorm); }



static BL d_cswap(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) { register F r = *re; *im = *re,*re = r; }
	return true; 
}

static BL d_cconj(I cnt,F *,I,F *im,I istr,F,F) { for(I i = 0; i < cnt; ++i,im += istr) *im *= -1.; return true; }

Vasp *Vasp::m_cswap() { return fc_arg("cswap",CX(),d_cswap); }
Vasp *Vasp::m_cconj() { return fc_arg("cconj",CX(),d_cconj); }




