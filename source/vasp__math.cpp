#include "main.h"
#include <math.h>

#define PI 3.1415926535897932385

inline F sgn(F x) { return x < 0.?-1.F:1.F; }


// just transform real vectors
Vasp *Vasp::fr_nop(const C *op,F v,V (*f)(F *,F,I))
{
	BL ok = true;
	for(I ci = 0; ok && ci < this->Vectors(); ++ci) {
		VBuffer *bref = this->Buffer(ci);		
		if(!bref->Data()) {
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			ok = false; // really return?
		}
		else {
			f(bref->Data(),v,bref->Frames());
			bref->Dirty();
		}
		delete bref;
	}

	// output Vasp
	return ok?new Vasp(*this):NULL;
}

// just transform complex vector pairs
Vasp *Vasp::fc_nop(const C *op,const CX &cx,V (*f)(F *,F *,F,F,I))
{
	BL ok = true;

	if(this->Vectors()%2 != 0) {
		post("%s(%s) - number of dst vectors is odd - omitting last vector",thisName(),op);
		// clear superfluous vector?
	}

	for(I ci = 0; ok && ci < this->Vectors()/2; ++ci) {
		VBuffer *bre = this->Buffer(ci*2),*bim = this->Buffer(ci*2+1); // complex channels

		if(!bre->Data()) {
			post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,bre->Name());
			ok = false; // really return?
		}
		else if(!bim->Data()) {
			post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,bim->Name());
			ok = false; // really return?
		}
		else {
			I frms = bre->Frames();
			if(frms != bim->Frames()) {
				post("%s - real/imag dst vector length is not equal - using minimum",thisName());
				frms = min(frms,bim->Frames());

				// clear the rest?
			}

			f(bre->Data(),bim->Data(),cx.real,cx.imag,frms);
			bre->Dirty(); bim->Dirty();
		}

		delete bre;
		delete bim;
	}

	// output corrected!! Vasp
	return ok?new Vasp(*this):NULL;
}

// just transform complex vector pairs
Vasp *Vasp::fc_nop(const C *op,const Argument &arg,const nop_funcs &f)
{
	if(arg.IsFloat()) 
		return fc_nop(op,CX(arg.GetFloat()),f);
	else if(arg.IsComplex()) 
		return fc_nop(op,arg.GetComplex(),f);
	else 
		return arg.IsNone()?new Vasp(*this):NULL;
}

/*
// just transform complex vector pairs
Vasp *Vasp::fc_nop(const C *op,I argc,t_atom *argv,const nop_funcs &f)
{
	F vr,vi = 0;
	if(IsFloat(argc,argv) || IsComplex(argc,argv)) {
		// complex (or real) input

		vr = flx::GetFloat(argv[0]);
		vi = argc >= 2?flx::GetFloat(argv[1]):0; 

		return fc_nop(op,vr,vi,f);
	}
	else if(argc >= 0) {
		post("%s(%s) - invalid argument",thisName(),op);
		return NULL;
	}

	return new Vasp(*this);
}
*/

// for real values or single real vector
Vasp *Vasp::fr_arg(const C *op,F v,V (*f)(F *,F,I))
{
	BL ok = true;
	for(I ci = 0; ok && ci < this->Vectors(); ++ci) {
		VBuffer *bref = this->Buffer(ci);		
		if(!bref->Data()) {
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			ok = false; // really return?
		}
		else {
			f(bref->Data(),v,bref->Frames());
			bref->Dirty();
		}
		delete bref;
	}

	// output Vasp
	return ok?new Vasp(*this):NULL;
}

Vasp *Vasp::fr_arg(const C *op,const Vasp &arg,V (*f)(F *,const F *,I))
{
	if(!arg.Ok()) {
		post("%s(%s) - invalid argument vasp detected and ignored",thisName(),op);
		return NULL;
	}

	if(arg.Vectors() > 1) {
		post("%s(%s) - using only first src vector",thisName(),op);
	}

	BL ok = true;

	VBuffer *barg = arg.Buffer(0);
	if(!barg->Data()) {
		post("%s(%s) - src vector (%s) is invalid",thisName(),op,barg->Name());
		ok = false; // really return?
	}
	else {
		I frms = barg->Frames();

		for(I ci = 0; ok && ci < this->Vectors(); ++ci) {
			VBuffer *bref = this->Buffer(ci);		

			if(!bref->Data()) {
				post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
				ok = false; // really break?
			}
			else {
				I frms1 = frms;
				if(frms1 != bref->Frames()) {
					post("%s(%s) - src/dst vector length not equal - using minimum",thisName(),op);
					frms1 = min(frms1,bref->Frames());

					// clear rest?
				}

				f(bref->Data(),barg->Data(),frms1);
				bref->Dirty();
			}

			delete bref;
		}
	}

	delete barg;

	// output corrected Vasp
	return ok?new Vasp(*this):NULL;
}

// for real values or single real vector
Vasp *Vasp::fr_arg(const C *op,const Argument &arg,const arg_funcs &f)
{
	if(arg.IsFloat()) 
		return fr_arg(op,arg.GetFloat(),f); // real input
	else if(arg.IsVasp()) 
		return fr_arg(op,arg.GetVasp(),f); // single vector source
	else
		return NULL;
}

/*
// for real values or single real vector
Vasp *Vasp::fr_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f)
{
	if(IsFloat(argc,argv)) {
		// real input
		F v = flx::GetFloat(argv[0]);
		return fr_arg(op,v,f);
	}
	else {
		// single vector source
		Vasp arg(argc,argv);
		return fr_arg(op,arg,f);
	}
}
*/

// for complex values or complex vector pair
Vasp *Vasp::fc_arg(const C *op,const CX &cx,V (*f)(F *,F *,F,F,I))
{
	// complex (or real) input

	if(this->Vectors()%2 != 0) {
		post("%s(%s) - number of src vectors is odd - omitting last vector",thisName(),op);
		// clear superfluous vector?
	}

	BL ok = true;

	for(I ci = 0; ci < this->Vectors()/2; ++ci) {
		VBuffer *bre = this->Buffer(ci*2),*bim = this->Buffer(ci*2+1); // complex channels

		if(!bre->Data()) {
			post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,bre->Name());
			ok = false;
		}
		if(!bim->Data()) {
			post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,bim->Name());
			ok = false;
		}
		else {
			I frms = bre->Frames();
			if(frms != bim->Frames()) {
				post("%s(%s) - real/imag dst vector length is not equal - using minimum",thisName(),op);
				frms = min(frms,bim->Frames());

				// clear the rest?
			}

			f(bre->Data(),bim->Data(),cx.real,cx.imag,frms);
			bre->Dirty(); bim->Dirty();
		}

		delete bre;
		delete bim;
	}

	// output corrected vasp
	return ok?new Vasp(*this):NULL;
}

// for complex values or complex vector pair
Vasp *Vasp::fc_arg(const C *op,const Vasp &arg,V (*f)(F *,F *,const F *,const F *,I))
{
	if(!arg.Ok()) {
		post("%s(%s) - invalid argument vasp detected and ignored",thisName(),op);
		return NULL;
	}

	BL ok = true;
	VBuffer *brarg = arg.Buffer(0),*biarg = arg.Buffer(1);

	if(!biarg) {
		post("%s(%s) - only one src vector - setting imaginary part to 0",thisName(),op);					
	}
	else if(arg.Vectors() > 2) {
		post("%s(%s) - using only first two src vectors",thisName(),op);
	}

	if(!brarg->Data()) {
		post("%s(%s) - real src vector (%s) is invalid",thisName(),op,brarg->Name());
		ok = false;
	}
	else if(biarg && !biarg->Data()) {
		post("%s(%s) - imag src vector (%s) is invalid",thisName(),op,biarg->Name());
		ok = false;
	}
	else {
		I frms = brarg->Frames();
		if(biarg && frms != biarg->Frames()) {
			post("%s(%s) - real/imag src vector length is not equal - using minimum",thisName(),op);
			frms = min(frms,biarg->Frames());
		}

		if(this->Vectors()%2 != 0) {
			post("%s(%s) - number of dst vectors is odd - omitting last vector",thisName(),op);
			// clear superfluous vector?
		}

		for(I ci = 0; ok && ci < this->Vectors()/2; ++ci) {
			VBuffer *brref = this->Buffer(ci*2),*biref = this->Buffer(ci*2+1);		

			if(!brref->Data()) {
				post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,brref->Name());
				ok = false; // really break?
			}
			else if(biref && !biref->Data()) {
				post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,biref->Name());
				ok = false; // really break?
			}
			else {
				I frms1 = frms;
				if(frms1 != brref->Frames() || frms1 != biref->Frames()) {
					post("%s(%s) - source/target vector length not equal - using minimum",thisName(),op);
					frms1 = min(frms1,min(biref->Frames(),brref->Frames()));

					// clear rest?
				}

				f(brref->Data(),biref->Data(),brarg->Data(),biarg->Data(),frms1);
				brref->Dirty(); biref->Dirty();
			}

			delete brref;
			delete biref;
		}
	}

	delete brarg;
	if(biarg) delete biarg;

	// output corrected!! Vasp
	return ok?new Vasp(*this):NULL;
}

Vasp *Vasp::fc_arg(const C *op,const Argument &arg,const arg_funcs &f)
{
	if(arg.IsFloat()) 
		return fc_arg(op,CX(arg.GetFloat()),f);
	else if(arg.IsComplex()) 
		return fc_arg(op,arg.GetComplex(),f);
	else if(arg.IsVasp()) 
		return fc_arg(op,arg.GetVasp(),f); // pairs of vectors
	else
		return NULL;
}

/*
Vasp *Vasp::fc_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f)
{
	if(IsFloat(argc,argv) || IsComplex(argc,argv)) {
		// complex (or real) input
		F vr = flx::GetFloat(argv[0]);
		F vi = argc >= 2?flx::GetFloat(argv[1]):0; // is zero if lacking
		return fc_arg(op,vr,vi,f);
	}
	else {
		// pairs of vectors
		Vasp arg(argc,argv);
		return fc_arg(op,arg,f);
	}
}
*/

// for multiple vectors
Vasp *Vasp::fm_arg(const C *op,const Vasp &arg,V (*f)(F *,const F *,I))
{
	if(!arg.Ok()) {
		post("%s(%s) - invalid src Vasp",thisName(),op);
		return NULL;
	}

	if(!this->Ok()) {
		post("%s(%s) - invalid dst Vasp",thisName(),op);
		return NULL;
	}

	I vecs = this->Vectors();
	if(vecs != arg.Vectors()) {
		post("%s(%s) - unequal src/dst vector count - using minimum",thisName(),op);

		vecs = min(vecs,arg.Vectors());
		// clear the rest?
	}

	BL ok = true,warned = false;

	// do
	for(I ci = 0; ok && ci < vecs; ++ci) {
		VBuffer *bref = this->Buffer(ci),*barg = arg.Buffer(ci);

		if(!bref->Data()) {
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			ok = false; // really return?
		}
		if(!barg->Data()) {
			post("%s(%s) - src vector (%s) is invalid",thisName(),op,barg->Name());
			ok = false; // really return?
		}
		else {
			I frms = bref->Frames();
			if(frms != barg->Frames()) {
				if(!warned) { // warn only once (or for each vector?) 
					post("%s(%s) - src/dst vector length not equal - using minimum",thisName(),op);
					warned = true;
				}

				frms = min(frms,barg->Frames());
				// clear the rest?
			}

			f(bref->Data(),barg->Data(),frms);
			bref->Dirty();
		}

		delete bref;
		delete barg;
	}

	// output corrected Vasp
	return ok?new Vasp(*this):NULL;
}

// for multiple vectors

Vasp *Vasp::fm_arg(const C *op,const Argument &arg,const arg_funcs &f)
{
	return arg.IsVasp()?fm_arg(op,arg.GetVasp(),f):NULL;
}
/*
Vasp *Vasp::fm_arg(const C *op,I argc,t_atom *argv,const arg_funcs &f)
{
	Vasp arg(argc,argv);
	return fm_arg(op,arg,f);
}
*/

static V d_copy(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] = val; }
static V d_copy(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] = src[i]; }
static V d_copy(F *rdst,F *idst,F re,F im,I cnt) { for(I i = 0; i < cnt; ++i) rdst[i] = re,idst[i] = im; }

static V d_copy(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) rdst[i] = rsrc[i],idst[i] = isrc[i]; 
	else
		for(I i = 0; i < cnt; ++i) rdst[i] = rsrc[i],idst[i] = 0.; 
}

static const Vasp::arg_funcs f_copy = { d_copy,d_copy,d_copy,d_copy };

Vasp *Vasp::m_copy(const Argument &arg) { return fr_arg("copy",arg,f_copy); }
Vasp *Vasp::m_ccopy(const Argument &arg) { return fc_arg("ccopy",arg,f_copy); }
Vasp *Vasp::m_mcopy(const Argument &arg) { return fm_arg("mcopy",arg,f_copy); }
/*
Vasp *Vasp::m_copy(I argc,t_atom *argv) { return fr_arg("copy",argc,argv,f_copy); }
Vasp *Vasp::m_ccopy(I argc,t_atom *argv) { return fc_arg("ccopy",argc,argv,f_copy); }
Vasp *Vasp::m_mcopy(I argc,t_atom *argv) { return fm_arg("mcopy",argc,argv,f_copy); }
*/

static V d_add(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] += v; }
static V d_add(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] += src[i]; }
static V d_add(F *rdst,F *idst,F re,F im,I cnt) { for(I i = 0; i < cnt; ++i) rdst[i] += re,idst[i] += im; }

static V d_add(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) rdst[i] += rsrc[i],idst[i] += isrc[i]; 
	else
		for(I i = 0; i < cnt; ++i) rdst[i] += rsrc[i]; 
}

static const Vasp::arg_funcs f_add = { d_add,d_add,d_add,d_add };

Vasp *Vasp::m_add(const Argument &arg) { return fr_arg("add",arg,f_add); }
Vasp *Vasp::m_cadd(const Argument &arg) { return fc_arg("cadd",arg,f_add); }
Vasp *Vasp::m_madd(const Argument &arg) { return fm_arg("madd",arg,f_add); }
/*
Vasp *Vasp::m_add(I argc,t_atom *argv) { return fr_arg("add",argc,argv,f_add); }
Vasp *Vasp::m_cadd(I argc,t_atom *argv) { return fc_arg("cadd",argc,argv,f_add); }
Vasp *Vasp::m_madd(I argc,t_atom *argv) { return fm_arg("madd",argc,argv,f_add); }
*/

static V d_sub(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] -= v; }
static V d_sub(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] -= src[i]; }
static V d_sub(F *rdst,F *idst,F re,F im,I cnt) { for(I i = 0; i < cnt; ++i) rdst[i] -= re,idst[i] -= im; }

static V d_sub(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc)
		for(I i = 0; i < cnt; ++i) rdst[i] -= rsrc[i],idst[i] -= isrc[i]; 
	else
		for(I i = 0; i < cnt; ++i) rdst[i] -= rsrc[i]; 
}

static const Vasp::arg_funcs f_sub = { d_sub,d_sub,d_sub,d_sub };

Vasp *Vasp::m_sub(const Argument &arg) { return fr_arg("sub",arg,f_sub); }
Vasp *Vasp::m_csub(const Argument &arg) { return fc_arg("csub",arg,f_sub); }
Vasp *Vasp::m_msub(const Argument &arg) { return fm_arg("msub",arg,f_sub); }


static V d_mul(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= v; }
static V d_mul(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= src[i]; }

static V d_mul(F *rdst,F *idst,F re,F im,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) {
		register F r = rdst[i]*re-idst[i]*im;
		idst[i] = idst[i]*re+rdst[i]*im;
		rdst[i] = r;
	}
}

static V d_mul(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
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
}

static const Vasp::arg_funcs f_mul = { d_mul,d_mul,d_mul,d_mul };

Vasp *Vasp::m_mul(const Argument &arg) { return fr_arg("mul",arg,f_mul); }
Vasp *Vasp::m_cmul(const Argument &arg) { return fc_arg("cmul",arg,f_mul); }
Vasp *Vasp::m_mmul(const Argument &arg) { return fm_arg("mmul",arg,f_mul); }


// how about div by 0?

static V d_div(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] /= v; }
static V d_div(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] /= src[i]; }

static V d_div(F *rdst,F *idst,F re,F im,I cnt) 
{ 
	F den = re*re+im*im;
	register F re1 = re/den,im1 = im/den;
	for(I i = 0; i < cnt; ++i) {
		register F r = rdst[i]*re1+idst[i]*im1;
		idst[i] = idst[i]*re1-rdst[i]*im1;
		rdst[i] = r;
	}
}

static V d_div(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
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
}

static const Vasp::arg_funcs f_div = { d_div,d_div,d_div,d_div };

Vasp *Vasp::m_div(const Argument &arg) { return fr_arg("div",arg,f_div); }
Vasp *Vasp::m_cdiv(const Argument &arg) { return fc_arg("cdiv",arg,f_div); }
Vasp *Vasp::m_mdiv(const Argument &arg) { return fm_arg("mdiv",arg,f_div); }




static V d_min(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst > val) *dst = val;
}

static V d_min(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst > *src) *dst = *src;
}

static const Vasp::arg_funcs f_min = { d_min,NULL,d_min,NULL };

Vasp *Vasp::m_min(const Argument &arg) { return fr_arg("min",arg,f_min); }
//Vasp *Vasp::m_cmin(const Argument &arg) { return fc_arg(argc,arg,d_min); }
Vasp *Vasp::m_mmin(const Argument &arg) { return fm_arg("mmin",arg,f_min); }



static V d_max(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst < val) *dst = val;
}

static V d_max(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst < *src) *dst = *src;
}

static const Vasp::arg_funcs f_max = { d_max,NULL,d_max,NULL };

Vasp *Vasp::m_max(const Argument &arg) { return fr_arg("max",arg,f_max); }
//Vasp *Vasp::m_cmax(const Argument &arg) { return fc_arg(arg,d_max,d_max); }
Vasp *Vasp::m_mmax(const Argument &arg) { return fm_arg("mmax",arg,f_max); }





static V d_pow(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)pow(fabs(*dst),val)*sgn(*dst);
}

static V d_sqr(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = *dst * *dst; }
static V d_ssqr(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)(*dst * fabs(*dst)); }

static V d_csqr(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		F r = *re * *re - *im * *im;
		*im *= *re * 2;
		*re = r;
	}
}

static V d_root(F *dst,F val,I cnt) 
{ 
	if(val == 0) return;
	F rad = 1.F/val;
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)pow(fabs(*dst),rad)*sgn(*dst);
}

static V d_sqrt(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)sqrt(fabs(*dst));
}

static V d_ssqrt(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = (F)sqrt(fabs(*dst))*sgn(*dst);
}

Vasp *Vasp::m_pow(F arg) { return fr_nop("pow",arg,d_pow); }
//Vasp *Vasp::m_cpow(I argc,t_atom *argv) { return fm_arg("cpow",argc,argv,d_max); }
Vasp *Vasp::m_sqr() { return fr_nop("sqr",0,d_sqr); }
Vasp *Vasp::m_ssqr() { return fr_nop("ssqr",0,d_ssqr); }
Vasp *Vasp::m_csqr() { return fc_nop("csqr",CX(),d_csqr); }
Vasp *Vasp::m_root(F arg) { return fr_nop("root",arg,d_root); }
Vasp *Vasp::m_sqrt() { return fr_nop("sqrt",0,d_sqrt); }
Vasp *Vasp::m_ssqrt() { return fr_nop("ssqrt",0,d_ssqrt); }


static V d_exp(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)exp(*dst); }

// how about numbers <= 0?
static V d_log(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)log(*dst); }

Vasp *Vasp::m_exp() { return fr_nop("exp",0,d_exp); }
Vasp *Vasp::m_log() { return fr_nop("log",0,d_log); }


static V d_inv(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = 1./ *dst; }

static V d_cinv(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		register F den = *re * *re+*im * *im;
		*re /= den,*im /= -den;
	}
}

Vasp *Vasp::m_inv() { return fr_nop("inv",0,d_inv); }
Vasp *Vasp::m_cinv() { return fc_nop("cinv",CX(),d_cinv); }



static V d_abs(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = fabs(*dst);
}

static V d_sign(F *dst,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = *dst == 0?0:(*dst < 0?-1.F:1.F);
}

static V d_polar(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		register F abs = *re * *re+*im * *im;
		if(*re)
			*im = atan(*im / *re);
		else
			if(abs) *im = *im > 0?PI/2:-PI/2;
		*re = abs;
	}
}

static V d_cart(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		register F arg = *im;
		*im = *re*sin(arg);
		*re *= cos(arg);
	}
}

Vasp *Vasp::m_abs() { return fr_nop("inv",0,d_inv); }
Vasp *Vasp::m_sign() { return fr_nop("sign",0,d_sign); }
Vasp *Vasp::m_polar() { return fc_nop("polar",CX(),d_polar); }
Vasp *Vasp::m_cart() { return fc_nop("cart",CX(),d_cart); }





static V d_norm(F *dst,F,I cnt) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i) { F s = fabs(dst[i]); if(s > v) v = s; }
	v = 1./v;
	for(i = 0; i < cnt; ++i) dst[i] *= v;
}

static V d_cnorm(F *re,F *im,F,F,I cnt) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i) { 
		register F abs = re[i]*re[i]+im[i]*im[i]; 
		if(abs > v) v = abs; 
	}
	v = 1./v;
	for(i = 0; i < cnt; ++i) re[i] *= v,im[i] *= v;
}

Vasp *Vasp::m_norm() { return fr_nop("norm",0,d_norm); }
Vasp *Vasp::m_cnorm() { return fc_nop("cnorm",CX(),d_cnorm); }



static V d_cswap(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) { register F r = *re; *im = *re,*re = r; }
}

static V d_cconj(F *,F *im,F,F,I cnt) { for(I i = 0; i < cnt; ++i) *im *= -1.; }

Vasp *Vasp::m_cswap() { return fc_nop("cswap",CX(),d_cswap); }
Vasp *Vasp::m_cconj() { return fc_nop("cconj",CX(),d_cconj); }




static V d_shift(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		if(ish > 0)
			for(I i = cnt-1; i >= ish; ++i,++dt) *dt = dt[-ish];
		else
			for(I i = ish; i < cnt; ++i,++dt) dt[-ish] = *dt;
	}
	else {
		// requires interpolation
		post("non-integer shift not implemented - truncating to integer");
		d_shift(dt,ish,cnt);
	}
}

static V d_xshift(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
		// no zero filling!

		I i,hcnt = cnt/2;
		// what to do if one sample remains? -> leave it in the middle

		if(ish > 0) {
			for(i = hcnt-1; i >= ish; ++i) dt[i] = dt[i-ish];
			for(i = (cnt-hcnt)+ish; i < cnt; ++i) dt[i-ish] = dt[i];
		}
		else {
			for(i = ish; i < hcnt; ++i) dt[i-ish] = dt[i];
			for(i = cnt-1; i >= (cnt-hcnt)+ish; ++i) dt[i] = dt[i-ish];
		}
	}
	else {
		// requires interpolation
		post("non-integer xshift not implemented - truncating to integer");
		d_xshift(dt,ish,cnt);
	}
}

static V d_rot(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
/*
		if(ish*2 > cnt) 
			// if more than half is rotated -> change direction
			d_rot(dt,ish-cnt,cnt);
		else {
*/
			if(ish > 0) {
				for(I i = cnt-1; i >= 0; ++i) {
				}
/*
	template<class _RI, class _Pd, class _Ty> inline
		void _Rotate(_RI _F, _RI _M, _RI _L, _Pd *, _Ty *)
		{_Pd _D = _M - _F;
		_Pd _N = _L - _F;
		for (_Pd _I = _D; _I != 0; )
			{_Pd _J = _N % _I;
			_N = _I, _I = _J; }
		if (_N < _L - _F)
			for (; 0 < _N; --_N)
				{_RI _X = _F + _N;
				_RI _Y = _X;
				_Ty _V = *_X;
				_RI _Z = _Y + _D == _L ? _F : _Y + _D;
				while (_Z != _X)
					{*_Y = *_Z;
					_Y = _Z;
					_Z = _D < _L - _Z ? _Z + _D
						: _F + (_D - (_L - _Z)); }
				*_Y = _V; }}

				}
*/
			}
			else {
				for(I i = ish; i < cnt-ish; ++i,++dt) dt[-ish] = *dt;
			}
//		}
	}
	else {
		// requires interpolation
		post("non-integer rot not implemented - truncating to integer");
		d_rot(dt,ish,cnt);
	}
}

static V d_xrot(F *dt,F sh,I cnt) 
{ 
	I ish = (I)sh;
	if(sh == ish) { // integer shift
	}
	else {
		// requires interpolation
		post("non-integer xrot not implemented - truncating to integer");
		d_xrot(dt,ish,cnt);
	}
}

static V d_mirr(F *dt,F,I cnt) 
{ 
}

static V d_xmirr(F *dt,F,I cnt) 
{ 
}


Vasp *Vasp::m_shift(F u) { return fr_nop("shift",u,d_shift); }
Vasp *Vasp::m_xshift(F u) { return fr_nop("xshift",u,d_xshift); }
Vasp *Vasp::m_rot(F u) { return fr_nop("rot",u,d_rot); }
Vasp *Vasp::m_xrot(F u) { return fr_nop("xrot",u,d_xrot); }
Vasp *Vasp::m_mirr() { return fr_nop("mirr",0,d_mirr); }
Vasp *Vasp::m_xmirr() { return fr_nop("xmirr",0,d_xmirr); } 


