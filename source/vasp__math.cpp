#include "main.h"
#include <math.h>

inline F sgn(F x) { return x < 0?-1:1; }





V vasp_modify::m_copy(I argc,t_atom *argv)
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
		return;
	}


}

V vasp_modify::fm_assign(I argc,t_atom *argv,V (*dofun)(F *,const F *,I))
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid source vasp",thisName());
		return;
	}

	if(!ref.Ok()) {
		post("%s - invalid target vasp",thisName());
		return;
	}

	if(ref.Channels() != arg.Channels()) {
		post("%s - unequal source/target channel count",thisName());
	}

	I ci,chns = min(ref.Channels(),arg.Channels());
	vbuffer **bref = new vbuffer *[chns];
	vbuffer **barg = new vbuffer *[chns];

	// set
	I frms = 1<<31,mxfrms = 0;
	for(ci = 0; ci < chns; ++ci) {
		bref[ci] = ref.Buffer(ci);
		if(bref[ci]->Frames() < frms) frms = bref[ci]->Frames();
		if(bref[ci]->Frames() > mxfrms) mxfrms = bref[ci]->Frames();

		barg[ci] = arg.Buffer(ci);
		if(barg[ci]->Frames() < frms) frms = barg[ci]->Frames();
		if(barg[ci]->Frames() > mxfrms) mxfrms = barg[ci]->Frames();
	}

	// do
	for(ci = 0; ci < chns; ++ci) {
		F *dst = bref[ci]->Data();
		const F *src = barg[ci]->Data();

		dofun(dst,src,frms);
	}

	if(frms < mxfrms) {
		post("%s - unequal frame count",thisName());
	}

	// delete
	for(ci = 0; ci < ref.Channels(); ++ci) {
		delete bref[ci];
		delete barg[ci];
	}
	delete[] bref;
	delete[] barg;
}


static V d_copy(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] = src[i]; }
static V d_copy(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] = val; }

/*
V vasp_modify::m_copy(I argc,t_atom *argv)
{
}
*/


V vasp_modify::m_mcopy(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_copy);
}



static V d_add(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] += src[i]; }
static V d_add(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] += val; }

V vasp_modify::m_add(I argc,t_atom *argv)
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
		return;
	}



}

V vasp_modify::m_madd(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_add);
}


static V d_sub(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] -= val; }
static V d_sub(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] -= src[i]; }

V vasp_modify::m_msub(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_sub);
}


static V d_mul(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= val; }
static V d_mul(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= src[i]; }

V vasp_modify::m_mmul(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_mul);
}


// how about div by 0?
static V d_div(F *dst,F val,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] /= val; }
static V d_div(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] /= src[i]; }

V vasp_modify::m_mdiv(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_div);
}



static V d_min(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst > val) *dst = val;
}

static V d_min(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst > *src) *dst = *src;
}

V vasp_modify::m_mmin(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_min);
}



static V d_max(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst < val) *dst = val;
}

static V d_max(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst < *src) *dst = *src;
}

V vasp_modify::m_mmax(I argc,t_atom *argv)
{
	fm_assign(argc,argv,d_max);
}





static V d_pow(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = pow(fabs(*dst),val)*sgn(*dst);
}

static V d_pow(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = pow(fabs(*dst),*src)*sgn(*dst);
}

static V d_sqr(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = *dst * *dst;
}

static V d_ssqr(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = *dst * fabs(*dst);
}

static V d_root(F *dst,F val,I cnt) 
{ 
	if(val == 0) return;
	F rad = 1./val;
	for(I i = 0; i < cnt; ++i,++dst) *dst = pow(fabs(*dst),rad)*sgn(*dst);
}

static V d_sqrt(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = sqrt(fabs(*dst));
}

static V d_ssqrt(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = sqrt(fabs(*dst))*sgn(*dst);
}







static V d_exp(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = exp(*dst);
}

// how about negative numbers?
static V d_log(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = log(*dst);
}




static V d_inv(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = 1./ *dst;
}



static V d_abs(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = fabs(*dst);
}

static V d_sign(F *dst,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) *dst = *dst == 0?0:(*dst < 0?-1:1);
}





static V d_norm(F *dst,I cnt) 
{ 
	F v = 0;
	I i;
	for(i = 0; i < cnt; ++i) { F s = fabs(dst[i]); if(s > v) v = s; }
	v = 1./v;
	for(i = 0; i < cnt; ++i) dst[i] *= v;
}


