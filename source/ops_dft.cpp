/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>


///////////////////////////////////////////////////////////////

static I radix2(I size)
{
  for(I i = 1,j = 1; i <= 31; i++)
  {
    j *= 2;
    if(j == size) return i;
  }
  return -1;
}

BL mixfft(I n,F *xRe,F *xIm,F *yRe,F *yIm);

static BL fft_fwd_real_any(I cnt,F *dt) 
{
	float *im,*tre,*tim;
	try {
		im = new float[cnt];
		tre = new float[cnt];
		tim = new float[cnt];
	}
	catch(...) {
		return false;
	}

	I i;
	for(i = 0; i < cnt; ++i) im[i] = 0;

	BL ret = mixfft(cnt,dt,im,tre,tim);

	if(ret) {
		F nrm = 1./sqrt(cnt);
		dt[0] = tre[0]*nrm; 
		for(i = 1; i < cnt/2; ++i) {
			dt[i] = tre[i]*nrm;
			dt[cnt-i] = tim[i]*nrm;
		}
		if(cnt%2 == 0) dt[i] = tre[i]*nrm;
	}

	delete[] im;
	delete[] tre;
	delete[] tim;
	return ret;
}

static BL fft_inv_real_any(I cnt,F *dt) 
{
	float *re,*im,*tim;
	try {
		re = new float[cnt];
		im = new float[cnt];
		tim = new float[cnt];
	}
	catch(...) {
		return false;
	}

	int i;
	re[0] = dt[0]; im[0] = 0;
	for(i = 1; i < cnt/2; ++i) {
		re[i] = re[cnt-i] = dt[i];
		im[i] = -(im[cnt-i] = dt[cnt-i]);
	}
	if(cnt%2 == 0) { re[i] = dt[i]; im[i] = 0; }

	BL ret = mixfft(cnt,re,im,dt,tim);

	F nrm = 1./sqrt(cnt);
	for(i = 0; i < cnt; ++i) dt[i] *= nrm;

	delete[] re;
	delete[] im;
	delete[] tim;
	return ret;
}

BL fft_fwd_real_radix2(I cnt,F *dt); 
BL fft_inv_real_radix2(I cnt,F *dt);

static BL fft_fwd_complex_any(I cnt,F *re,F *im) 
{
	float *tre,*tim;
	try {
		tre = new float[cnt];
		tim = new float[cnt];
	}
	catch(...) {
		return false;
	}
	BL ret = mixfft(cnt,re,im,tre,tim);

	if(ret) {
		F nrm = 1./sqrt(cnt);
		for(I i = 0; i < cnt; ++i) {
			re[i] = tre[i]*nrm;
			im[i] = tim[i]*nrm;
		}
	}

	delete[] tre;
	delete[] tim;
	return ret;
}

static BL fft_inv_complex_any(I cnt,F *re,F *im) 
{
	I i;
	for(i = 0; i < cnt; ++i) im[i] = -im[i];
	BL ret = fft_fwd_complex_any(cnt,re,im);
	if(ret) {
		for(i = 0; i < cnt; ++i) im[i] = -im[i];
	}
	return ret;
}

BL fft_fwd_complex_radix2(I cnt,F *re,F *im); 
BL fft_inv_complex_radix2(I cnt,F *re,F *im);

///////////////////////////////////////////////////////////////

static BL d_rfft(F *dt,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_fwd_real_radix2(cnt,dt);
		else
			return fft_fwd_real_any(cnt,dt);
	else
		return true;
}

static BL d_rifft(F *dt,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_inv_real_radix2(cnt,dt);
		else
			return fft_inv_real_any(cnt,dt);
	else
		return true;
}

Vasp *Vasp::m_rfft() { return fr_nop("rfft",0,d_rfft); }
Vasp *Vasp::m_rifft() { return fr_nop("rifft",0,d_rifft); }


static BL d_cfft(F *re,F *im,F,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_fwd_complex_radix2(cnt,re,im);
		else
			return fft_fwd_complex_any(cnt,re,im);
	else
		return true;
}

static BL d_cifft(F *re,F *im,F,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1) 
			return fft_inv_complex_radix2(cnt,re,im);
		else
			return fft_inv_complex_any(cnt,re,im);
	else
		return true;
}

Vasp *Vasp::m_cfft() { return fc_nop("cfft",0,d_cfft); }
Vasp *Vasp::m_cifft() { return fc_nop("cifft",0,d_cifft); } 


