/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>


Vasp *VaspOp::m_run(Vasp &src,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);

	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}


Vasp *VaspOp::m_cun(Vasp &src,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);

	CVecBlock *vecs = GetCVecs(p.opname,src,dst);
	if(vecs) {
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}



Vasp *VaspOp::m_rbin(Vasp &src,const Argument &arg,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);
	BL argvasp = arg.IsVasp();

	RVecBlock *vecs = argvasp?GetRVecs(p.opname,src,arg.GetVasp(),dst):GetRVecs(p.opname,src,dst);
	if(vecs) {
		if(!argvasp) p.rbin.arg = arg.GetADouble(); // if no arg vasp
	
		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}

Vasp *VaspOp::m_cbin(Vasp &src,const Argument &arg,Vasp *dst,VecOp::opfun *fun,const C *opnm) 
{ 
	Vasp *ret = NULL;
	OpParam p(opnm);
	BL argvasp = arg.IsVasp();

	CVecBlock *vecs = argvasp?GetCVecs(p.opname,src,arg.GetVasp(),dst):GetCVecs(opnm,src,dst);
	if(vecs) {
		if(!argvasp) {
			CX z = arg.GetAComplex();
			p.cbin.rarg = z.real; 
			p.cbin.iarg = z.imag; 
		}

		ret = DoOp(vecs,fun,p);
		delete vecs;
	}

	return ret;
}



// \remark sign is preserved
BL VecOp::d_pow(OpParam &p) 
{ 
	const R v = p.rbin.arg;
	for(I i = 0; i < p.frames; ++i,p.rsdt += p.rss,p.rddt += p.rds) *p.rddt = (F)pow(fabs(*p.rsdt),v)*sgn(*p.rsdt);
	return true; 
}


BL VecOp::d_sqr(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = *dst * *dst; 
	return true; 
}

BL VecOp::d_ssqr(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)(*dst * fabs(*dst)); 
	return true; 
}

BL VecOp::d_csqr(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		F r = *re * *re - *im * *im;
		*im *= *re * 2;
		*re = r;
	}
	return true; 
}

BL VecOp::d_sqrt(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)sqrt(fabs(*dst));
	return true; 
}

BL VecOp::d_ssqrt(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)sqrt(fabs(*dst))*sgn(*dst);
	return true; 
}


BL VecOp::d_exp(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)exp(*dst); 
	return true; 
}

// how about numbers <= 0?
BL VecOp::d_log(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = (F)log(*dst); 
	return true; 
}

BL VecOp::d_inv(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = 1./ *dst; 
	return true; 
}

BL VecOp::d_cinv(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F den = abs(*re,*im);
		*re /= den,*im /= -den;
	}
	return true; 
}

BL VecOp::d_abs(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = fabs(*dst);
	return true; 
}

BL VecOp::d_sign(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,dst += str) *dst = *dst == 0?0:(*dst < 0?-1.F:1.F);
	return true; 
}

BL VecOp::d_polar(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F _abs = abs(*re,*im);
		*im = arg(*re,*im);
		*re = _abs;
	}
	return true; 
}

BL VecOp::d_cart(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		register F arg = *im;
		*im = *re*sin(arg);
		*re *= cos(arg);
	}
	return true; 
}


BL VecOp::d_opt(OpParam &p) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i,dst += str) { F s = fabs(*dst); if(s > v) v = s; }

	if(v && v != 1) {
		v = 1./v;
		for(i = 0; i < cnt; ++i) *(dst -= str) *= v;
	}
	return true; 
}

BL VecOp::d_copt(OpParam &p) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i,re += rstr,im += istr) { 
		register F abs = *re * *re+*im * *im; 
		if(abs > v) v = abs; 
	}

	if(v && v != 1) {
		v = 1./sqrt(v);
		for(i = 0; i < cnt; ++i) *(re -= rstr) *= v,*(im -= istr) *= v;
	}
	return true; 
}

BL VecOp::d_cswap(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) { register F r = *re; *im = *re,*re = r; }
	return true; 
}

BL VecOp::d_cconj(OpParam &p) 
{ 
	for(I i = 0; i < cnt; ++i,im += istr) *im *= -1.; 
	return true; 
}





