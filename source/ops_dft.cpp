/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "ops_dft.h"
#include <math.h>


///////////////////////////////////////////////////////////////

static I radix2(I size)
{
	I i,j;
	for(i = j = 1; j < size; i++,j *= 2);
	return j == size?i:-1;
}

BL mixfft(I n,F *xRe,F *xIm,F *yRe,F *yIm);

static BL fft_fwd_real_any(I cnt,S *src,I sstr,S *dst = NULL,I dstr = 0) 
{
	BL ret;
	F *re,*im,*tre,*tim;
//	try {
		if(!dst) dst = src,dstr = sstr;
		re = sstr == 1?src:new float[cnt];
		im = new float[cnt];
		tre = dstr == 1?dst:new float[cnt];
		tim = new float[cnt];
/*
	}
	catch(...) {
		return false;
	}
*/
	if(re && im && tre && tim) {
		I i;
		if(re != src) {
			for(i = 0; i < cnt; ++i) re[i] = src[i*sstr];
		}
		for(i = 0; i < cnt; ++i) im[i] = 0;

		ret = mixfft(cnt,re,im,tre,tim);

		if(ret) {
			F nrm = 1./sqrt(cnt);

			if(dst == tre) {
				tre[0] *= nrm; 
				for(i = 1; i < cnt/2; ++i) {
					tre[i] *= nrm;
					tre[cnt-i] = tim[i]*nrm;
				}
				if(cnt%2 == 0) tre[i] *= nrm;
			}
			else {
				dst[0] = tre[0]*nrm; 
				for(i = 1; i < cnt/2; ++i) {
					dst[i*dstr] = tre[i]*nrm;
					dst[(cnt-i)*dstr] = tim[i]*nrm;
				}
				if(cnt%2 == 0) dst[i*dstr] = tre[i]*nrm;
			}
		}
	}
	else 
		ret = false;

	if(re && re != src) delete[] re;
	if(im) delete[] im;
	if(tre && tre != dst) delete[] tre;
	if(tim) delete[] tim;
	return ret;
}

static BL fft_inv_real_any(I cnt,F *src,I sstr,F *dst = NULL,I dstr = 0) 
{
	BL ret;
	float *re,*im,*tre,*tim;
//	try {
		if(!dst) dst = src,dstr = sstr;
		re = sstr == 1?src:new float[cnt];
		im = new float[cnt];
		tre = dstr == 1?dst:new float[cnt];
		tim = new float[cnt];
/*
	}
	catch(...) {
		return false;
	}
*/
	if(re && im && tre && tim) {
		int i;

		if(re != src) {
			re[0] = src[0]; im[0] = 0;
			for(i = 1; i < cnt/2; ++i) {
				re[i] = re[cnt-i] = src[i*sstr];
				im[i] = -(im[cnt-i] = src[(cnt-i)*sstr]);
			}
			if(cnt%2 == 0) { re[i] = src[i*sstr]; im[i] = 0; }
		}
		else {
			im[0] = 0;
			for(i = 1; i < cnt/2; ++i) im[i] = -(im[cnt-i] = re[cnt-i]);
			for(i = 1; i < cnt/2; ++i) re[cnt-i] = re[i];
			if(cnt%2 == 0) im[i] = 0;
		}

		ret = mixfft(cnt,re,im,tre,tim);

		F nrm = 1./sqrt(cnt);
		if(tre == dst)
			for(i = 0; i < cnt; ++i) dst[i] *= nrm;
		else 
			for(i = 0; i < cnt; ++i) dst[i*dstr] = tre[i]*nrm;
	}
	else
		return false;

	if(re && re != dst) delete[] re;
	if(im) delete[] im;
	if(tre && tre != src) delete[] tre;
	if(tim) delete[] tim;
	return ret;
}

//BL fft_fwd_real_radix2(I cnt,F *src,F *dst = NULL); 
//BL fft_inv_real_radix2(I cnt,F *src,F *dst = NULL);
BL fft_fwd_real_radix2(I cnt,F *dt); //,I sstr,F *dst = NULL,I dstr = 0); 
BL fft_inv_real_radix2(I cnt,F *dt); //,I sstr,F *dst = NULL,I dstr = 0);

static BL fft_fwd_complex_any(I cnt,F *rsdt,F *isdt,F *rddt,F *iddt) 
#if 1
{
	BL ret = mixfft(cnt,rsdt,isdt,rddt,iddt);

	if(ret) {
		F nrm = 1./sqrt(cnt);
		for(I i = 0; i < cnt; ++i) rddt[i] *= nrm,iddt[i] *= nrm;
	}

	return ret;
}
#else
{
	float *tre = NULL,*tim = NULL;
#if 1
	try {
		tre = new float[cnt];
		tim = new float[cnt];
	}
	catch(...) {
		return false;
	}
#else
	tre = re,tim = im;
#endif

	BL ret = mixfft(cnt,re,im,tre,tim);

	if(ret) {
		F nrm = 1./sqrt(cnt);
		for(I i = 0; i < cnt; ++i) {
			re[i] = tre[i]*nrm;
			im[i] = tim[i]*nrm;
		}
	}

#if 1
	delete[] tre;
	delete[] tim;
#endif
	return ret;
}
#endif

static BL fft_inv_complex_any(I cnt,F *rsdt,F *isdt,F *rddt,F *iddt) 
#if 1
{
	I i;
	for(i = 0; i < cnt; ++i) isdt[i] = -isdt[i];
	BL ret = fft_fwd_complex_any(cnt,rsdt,isdt,rddt,iddt);
	if(ret) {
		for(i = 0; i < cnt; ++i) iddt[i] = -iddt[i];
	}
	if(isdt != iddt) {
		for(i = 0; i < cnt; ++i) isdt[i] = -isdt[i];
	}
	return ret;
}
#else
{
	I i;
	for(i = 0; i < cnt; ++i) im[i] = -im[i];
	BL ret = fft_fwd_complex_any(cnt,re,im);
	if(ret) {
		for(i = 0; i < cnt; ++i) im[i] = -im[i];
	}
	return ret;
}
#endif

BL fft_fwd_complex_radix2(I cnt,F *re,F *im); 
BL fft_inv_complex_radix2(I cnt,F *re,F *im);
BL fft_fwd_complex_radix2(I cnt,F *re,I rstr,F *im,I istr); 
BL fft_inv_complex_radix2(I cnt,F *re,I rstr,F *im,I istr);

///////////////////////////////////////////////////////////////

static BL d_rfft(I cnt,S *dt,I str) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_fwd_real_radix2(cnt,dt); //,str);
		else
			return fft_fwd_real_any(cnt,dt,str);
	else
		return true;
}

static BL d_rifft(I cnt,S *dt,I str) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_inv_real_radix2(cnt,dt); //,str);
		else
			return fft_inv_real_any(cnt,dt,str);
	else
		return true;
}

/*
Vasp *Vasp::m_rfft() { return fr_arg("rfft",0,d_rfft); }
Vasp *Vasp::m_rifft() { return fr_arg("rifft",0,d_rifft); }
*/

static BL d_cfft(I cnt,S *rsdt,I rss,S *isdt,I iss,S *rddt,I rds,S *iddt,I ids) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_fwd_complex_radix2(cnt,rsdt,isdt);
		else
			return fft_fwd_complex_any(cnt,rsdt,isdt,rddt,iddt);
	else
		return true;
}

static BL d_cifft(I cnt,S *rsdt,I rss,S *isdt,I iss,S *rddt,I rds,S *iddt,I ids)
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_inv_complex_radix2(cnt,rsdt,isdt);
		else
			return fft_inv_complex_any(cnt,rsdt,isdt,rddt,iddt);
	else
		return true;
}



Vasp *VaspOp::m_rfft(OpParam &p,Vasp &src,Vasp *dst,BL inv) 
{ 
/*
	if(dst && dst->Ok()) {
		error("rfft: out-of-place operation not implemented yet");
		return NULL;
	}
*/
	RVecBlock *vecs = GetRVecs(p.opname,src,dst);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Vecs(); ++i) {
			VBuffer *s = vecs->Src(i);
			if(inv)
				ok = d_rifft(vecs->Frames(),s->Pointer(),s->Channels());
			else
				ok = d_rfft(vecs->Frames(),s->Pointer(),s->Channels());
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}

Vasp *VaspOp::m_cfft(OpParam &p,Vasp &src,Vasp *dst,BL inv) 
{ 
/*
	if(dst && dst->Ok()) {
		error("rfft: out-of-place operation not implemented yet");
		return NULL;
	}
*/
	CVecBlock *vecs = GetCVecs(p.opname,src,dst,true);
	if(vecs) {
		BL ok = true;
		for(I i = 0; ok && i < vecs->Pairs(); ++i) {
			VBuffer *sre = vecs->ReSrc(i),*sim = vecs->ImSrc(i);
			VBuffer *dre = vecs->ReDst(i),*dim = vecs->ImDst(i);
			if(!dre) dre = sre;
			if(!dim) dim = sim;
			if(inv)
				ok = d_cifft(vecs->Frames(),sre->Pointer(),sre->Channels(),sim?sim->Pointer():NULL,sim?sim->Channels():0,dre->Pointer(),dre->Channels(),dim?dim->Pointer():NULL,dim?dim->Channels():0);
			else
				ok = d_cfft(vecs->Frames(),sre->Pointer(),sre->Channels(),sim?sim->Pointer():NULL,sim?sim->Channels():0,dre->Pointer(),dre->Channels(),dim?dim->Pointer():NULL,dim?dim->Channels():0);
		}
		return ok?vecs->ResVasp():NULL;
	}
	else
		return NULL;
}

VASP_UNARY("vasp.rfft",rfft,true,"Real DFT")
VASP_UNARY("vasp.r!fft",rifft,true,"Real inverse DFT")
VASP_UNARY("vasp.cfft",cfft,true,"Complex DFT")
VASP_UNARY("vasp.c!fft",cifft,true,"Complex inverse DFT")


