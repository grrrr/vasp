#include "main.h"
#include <math.h>

inline F sgn(F x) { return x < 0.?-1.F:1.F; }

#define PI 3.1415926535897932385

// just transform real vectors
V vasp_tx::fr_transf(const C *op,F v,V (*dofunR)(F *,F,I))
{
	for(I ci = 0; ci < ref.Vectors(); ++ci) {
		vbuffer *bref = ref.Buffer(ci);		
		if(!bref->Data())
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
		else {
			dofunR(bref->Data(),v,bref->Frames());
			bref->Dirty();
		}
		delete bref;
	}

	// output vasp
}

// just transform complex vector pairs
V vasp_tx::fc_transf(const C *op,I argc,t_atom *argv,V (*dofunC)(F *,F *,F,F,I))
{
	F vr,vi = 0;
	if(argc > 0 && 
		(
			(argc == 1 && ISFLOAT(argv[0])) || 
			(argc == 2 && ISFLOAT(argv[0]) && ISFLOAT(argv[1]))
		)
	) {
		// complex (or real) input

		vr = atom_getfloatarg(1,argc,argv);
		vi = atom_getfloatarg(2,argc,argv); // is zero if absent
	}
	else if(argc >= 0) {
		post("%s(%s) - invalid argument",thisName(),op);
		return;
	}

	if(ref.Vectors()%2 != 0) {
		post("%s(%s) - number of dst vectors is odd - omitting last vector",thisName(),op);
		// clear superfluous vector?
	}

	for(I ci = 0; ci < ref.Vectors()/2; ++ci) {
		vbuffer *bre = ref.Buffer(ci*2),*bim = ref.Buffer(ci*2+1); // complex channels

		if(!bre->Data())
			post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,bre->Name());
		else if(!bim->Data())
			post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,bim->Name());
		else {
			I frms = bre->Frames();
			if(frms != bim->Frames()) {
				post("%s - real/imag dst vector length is not equal - using minimum",thisName());
				frms = min(frms,bim->Frames());

				// clear the rest?
			}

			dofunC(bre->Data(),bim->Data(),vr,vi,frms);
			bre->Dirty(); bim->Dirty();
		}

		delete bre;
		delete bim;
	}

	// output corrected vasp
}



// for real values or single real vector
V vasp_tx::fr_assign(const C *op,I argc,t_atom *argv,V (*dofunV)(F *,const F *,I),V (*dofunR)(F *,F,I))
{
	if(argc == 1 && ISFLOAT(argv[0])) {
		// real input
		F v = atom_getfloatarg(0,argc,argv);

		for(I ci = 0; ci < ref.Vectors(); ++ci) {
			vbuffer *bref = ref.Buffer(ci);		
			if(!bref->Data())
				post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
			else {
				dofunR(bref->Data(),v,bref->Frames());
				bref->Dirty();
			}
			delete bref;
		}

		// output vasp
	}
	else {
		// single vector source

		vasp arg(argc,argv);
		if(!arg.Ok()) {
			post("%s(%s) - invalid vasp detected and ignored",thisName(),op);
			return;
		}

		if(arg.Vectors() > 1) {
			post("%s(%s) - using only first src vector",thisName(),op);
		}

		vbuffer *barg = arg.Buffer(0);
		if(!barg->Data())
			post("%s(%s) - src vector (%s) is invalid",thisName(),op,barg->Name());
		else {
			I frms = barg->Frames();

			for(I ci = 0; ci < ref.Vectors(); ++ci) {
				vbuffer *bref = ref.Buffer(ci);		

				if(!bref->Data())
					post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
				else {
					I frms1 = frms;
					if(frms1 != bref->Frames()) {
						post("%s(%s) - src/dst vector length not equal - using minimum",thisName(),op);
						frms1 = min(frms1,bref->Frames());

						// clear rest?
					}

					dofunV(bref->Data(),barg->Data(),frms1);
					bref->Dirty();
				}

				delete bref;
			}
		}

		delete barg;

		// output corrected vasp
	}
}

// for complex values or complex vector pair
V vasp_tx::fc_assign(const C *op,I argc,t_atom *argv,V (*dofunCV)(F *,F *,const F *,const F *,I),V (*dofunC)(F *,F *,F,F,I))
{
	if((argc == 1 && ISFLOAT(argv[0])) || (argc == 2 && ISFLOAT(argv[0]) && ISFLOAT(argv[1]))) {
		// complex (or real) input

		F vr = atom_getfloatarg(1,argc,argv);
		F vi = atom_getfloatarg(2,argc,argv); // is zero if lacking

		if(ref.Vectors()%2 != 0) {
			post("%s(%s) - number of src vectors is odd - omitting last vector",thisName(),op);
			// clear superfluous vector?
		}

		for(I ci = 0; ci < ref.Vectors()/2; ++ci) {
			vbuffer *bre = ref.Buffer(ci*2),*bim = ref.Buffer(ci*2+1); // complex channels

			if(!bre->Data())
				post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,bre->Name());
			if(!bim->Data())
				post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,bim->Name());
			else {
				I frms = bre->Frames();
				if(frms != bim->Frames()) {
					post("%s(%s) - real/imag dst vector length is not equal - using minimum",thisName(),op);
					frms = min(frms,bim->Frames());

					// clear the rest?
				}

				dofunC(bre->Data(),bim->Data(),vr,vi,frms);
				bre->Dirty(); bim->Dirty();
			}

			delete bre;
			delete bim;
		}

		// output corrected vasp
	}
	else {
		// pairs of vectors

		vasp arg(argc,argv);
		if(!arg.Ok()) {
			post("%s(%s) - invalid vasp detected and ignored",thisName(),op);
			return;
		}

		vbuffer *brarg = arg.Buffer(0),*biarg = arg.Buffer(1);

		if(!biarg) {
			post("%s(%s) - only one src vector - setting imaginary part to 0",thisName(),op);					
		}
		else if(arg.Vectors() > 2) {
			post("%s(%s) - using only first two src vectors",thisName(),op);
		}

		if(!brarg->Data())
			post("%s(%s) - real src vector (%s) is invalid",thisName(),op,brarg->Name());
		else if(biarg && !biarg->Data())
			post("%s(%s) - imag src vector (%s) is invalid",thisName(),op,biarg->Name());
		else {
			I frms = brarg->Frames();
			if(biarg && frms != biarg->Frames()) {
				post("%s(%s) - real/imag src vector length is not equal - using minimum",thisName(),op);
				frms = min(frms,biarg->Frames());
			}

			if(ref.Vectors()%2 != 0) {
				post("%s(%s) - number of dst vectors is odd - omitting last vector",thisName(),op);
				// clear superfluous vector?
			}

			for(I ci = 0; ci < ref.Vectors()/2; ++ci) {
				vbuffer *brref = ref.Buffer(ci*2),*biref = ref.Buffer(ci*2+1);		

				if(!brref->Data())
					post("%s(%s) - real dst vector (%s) is invalid",thisName(),op,brref->Name());
				else if(biref && !biref->Data())
					post("%s(%s) - imag dst vector (%s) is invalid",thisName(),op,biref->Name());
				else {
					I frms1 = frms;
					if(frms1 != brref->Frames() || frms1 != biref->Frames()) {
						post("%s(%s) - source/target vector length not equal - using minimum",thisName(),op);
						frms1 = min(frms1,min(biref->Frames(),brref->Frames()));

						// clear rest?
					}

					dofunCV(brref->Data(),biref->Data(),brarg->Data(),biarg->Data(),frms1);
					brref->Dirty(); biref->Dirty();
				}

				delete brref;
				delete biref;
			}
		}

		delete brarg;
		if(biarg) delete biarg;

		// output corrected vasp
	}
}

// for multiple vectors
V vasp_tx::fm_assign(const C *op,I argc,t_atom *argv,V (*dofun)(F *,const F *,I))
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s(%s) - invalid src vasp",thisName(),op);
		return;
	}

	if(!ref.Ok()) {
		post("%s(%s) - invalid dst vasp",thisName(),op);
		return;
	}

	I vecs = ref.Vectors();
	if(vecs != arg.Vectors()) {
		post("%s(%s) - unequal src/dst vector count - using minimum",thisName(),op);

		vecs = min(vecs,arg.Vectors());
		// clear the rest?
	}

	BL warned = false;

	// do
	for(I ci = 0; ci < vecs; ++ci) {
		vbuffer *bref = ref.Buffer(ci),*barg = arg.Buffer(ci);

		if(!bref->Data())
			post("%s(%s) - dst vector (%s) is invalid",thisName(),op,bref->Name());
		if(!barg->Data())
			post("%s(%s) - src vector (%s) is invalid",thisName(),op,barg->Name());
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

			dofun(bref->Data(),barg->Data(),frms);
			bref->Dirty();
		}

		delete bref;
		delete barg;
	}

	// output corrected vasp
}


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

V vasp_tx::m_copy(I argc,t_atom *argv) { fr_assign("copy",argc,argv,d_copy,d_copy); }
V vasp_tx::m_ccopy(I argc,t_atom *argv) { fc_assign("ccopy",argc,argv,d_copy,d_copy); }
V vasp_tx::m_mcopy(I argc,t_atom *argv) { fm_assign("mcopy",argc,argv,d_copy); }


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

V vasp_tx::m_add(I argc,t_atom *argv) { fr_assign("add",argc,argv,d_add,d_add); }
V vasp_tx::m_cadd(I argc,t_atom *argv) { fc_assign("cadd",argc,argv,d_add,d_add); }
V vasp_tx::m_madd(I argc,t_atom *argv) { fm_assign("madd",argc,argv,d_add); }


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

V vasp_tx::m_sub(I argc,t_atom *argv) { fr_assign("sub",argc,argv,d_sub,d_sub); }
V vasp_tx::m_csub(I argc,t_atom *argv) { fc_assign("csub",argc,argv,d_sub,d_sub); }
V vasp_tx::m_msub(I argc,t_atom *argv) { fm_assign("msub",argc,argv,d_sub); }


static V d_mul(F *dst,F v,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= v; }
static V d_mul(F *dst,const F *src,I cnt) { for(I i = 0; i < cnt; ++i) dst[i] *= src[i]; }

static V d_mul(F *rdst,F *idst,F re,F im,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) {
		register F re = rdst[i]*re-idst[i]*im;
		idst[i] = idst[i]*re+rdst[i]*im;
		rdst[i] = re;
	}
}

static V d_mul(F *rdst,F *idst,const F *rsrc,const F *isrc,I cnt) 
{ 
	if(isrc) 
		for(I i = 0; i < cnt; ++i) {
			register F re = rdst[i]*rsrc[i]-idst[i]*isrc[i];
			idst[i] = idst[i]*rsrc[i]+rdst[i]*isrc[i];
			rdst[i] = re;
		}
	else 
		for(I i = 0; i < cnt; ++i) 
			rdst[i] *= rsrc[i],idst[i] *= rsrc[i];
}

V vasp_tx::m_mul(I argc,t_atom *argv) { fr_assign("mul",argc,argv,d_mul,d_mul); }
V vasp_tx::m_cmul(I argc,t_atom *argv) { fc_assign("cmul",argc,argv,d_mul,d_mul); }
V vasp_tx::m_mmul(I argc,t_atom *argv) { fm_assign("mmul",argc,argv,d_mul); }


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

V vasp_tx::m_div(I argc,t_atom *argv) { fr_assign("div",argc,argv,d_div,d_div); }
V vasp_tx::m_cdiv(I argc,t_atom *argv) { fc_assign("cdiv",argc,argv,d_div,d_div); }
V vasp_tx::m_mdiv(I argc,t_atom *argv) { fm_assign("mdiv",argc,argv,d_div); }




static V d_min(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst > val) *dst = val;
}

static V d_min(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst > *src) *dst = *src;
}

V vasp_tx::m_min(I argc,t_atom *argv) { fr_assign("min",argc,argv,d_min,d_min); }
//V vasp_tx::m_cmin(I argc,t_atom *argv) { fc_assign(argc,argv,d_min,d_min); }
V vasp_tx::m_mmin(I argc,t_atom *argv) { fm_assign("mmin",argc,argv,d_min); }



static V d_max(F *dst,F val,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst) if(*dst < val) *dst = val;
}

static V d_max(F *dst,const F *src,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++dst,++src) if(*dst < *src) *dst = *src;
}

V vasp_tx::m_max(I argc,t_atom *argv) { fr_assign("max",argc,argv,d_max,d_max); }
//V vasp_tx::m_cmax(I argc,t_atom *argv) { fc_assign(argc,argv,d_max,d_max); }
V vasp_tx::m_mmax(I argc,t_atom *argv) { fm_assign("mmax",argc,argv,d_max); }





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

V vasp_tx::m_pow(F arg) { fr_transf("pow",arg,d_pow); }
//V vasp_tx::m_cpow(I argc,t_atom *argv) { fm_assign("cpow",argc,argv,d_max); }
V vasp_tx::m_sqr() { fr_transf("sqr",0,d_sqr); }
V vasp_tx::m_ssqr() { fr_transf("ssqr",0,d_ssqr); }
V vasp_tx::m_csqr() { fc_transf("csqr",-1,NULL,d_csqr); }
V vasp_tx::m_root(F arg) { fr_transf("root",arg,d_root); }
V vasp_tx::m_sqrt() { fr_transf("sqrt",0,d_sqrt); }
V vasp_tx::m_ssqrt() { fr_transf("ssqrt",0,d_ssqrt); }


static V d_exp(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)exp(*dst); }

// how about numbers <= 0?
static V d_log(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = (F)log(*dst); }

V vasp_tx::m_exp() { fr_transf("exp",0,d_exp); }
V vasp_tx::m_log() { fr_transf("log",0,d_log); }


static V d_inv(F *dst,F,I cnt) { for(I i = 0; i < cnt; ++i,++dst) *dst = 1./ *dst; }

static V d_cinv(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i,++re,++im) {
		register F den = *re * *re+*im * *im;
		*re /= den,*im /= -den;
	}
}

V vasp_tx::m_inv() { fr_transf("inv",0,d_inv); }
V vasp_tx::m_cinv() { fc_transf("cinv",-1,NULL,d_cinv); }



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

V vasp_tx::m_abs() { fr_transf("inv",0,d_inv); }
V vasp_tx::m_sign() { fr_transf("sign",0,d_sign); }
V vasp_tx::m_polar() { fc_transf("polar",-1,NULL,d_polar); }
V vasp_tx::m_cart() { fc_transf("cart",-1,NULL,d_cart); }





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

V vasp_tx::m_norm() { fr_transf("norm",0,d_norm); }
V vasp_tx::m_cnorm() { fc_transf("cnorm",-1,NULL,d_cnorm); }



static V d_cswap(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) { register F r = *re; *im = *re,*re = r; }
}

static V d_cconj(F *,F *im,F,F,I cnt) { for(I i = 0; i < cnt; ++i) *im *= -1.; }

V vasp_tx::m_cswap() { fc_transf("cswap",-1,NULL,d_cswap); }
V vasp_tx::m_cconj() { fc_transf("cconj",-1,NULL,d_cconj); }




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


V vasp_tx::m_shift(F u) { fr_transf("shift",u,d_shift); }
V vasp_tx::m_xshift(F u) { fr_transf("xshift",u,d_xshift); }
V vasp_tx::m_rot(F u) { fr_transf("rot",u,d_rot); }
V vasp_tx::m_xrot(F u) { fr_transf("xrot",u,d_xrot); }
V vasp_tx::m_mirr() { fr_transf("mirr",0,d_mirr); }
V vasp_tx::m_xmirr() { fr_transf("xmirr",0,d_xmirr); } 


