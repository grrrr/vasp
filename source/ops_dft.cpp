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

V mixfft(I n,F *xRe,F *xIm,F *yRe,F *yIm);

static BL fft_fwd_real_any(I cnt,F *dt) 
{
	if(cnt%2 == 1) {
		post("FFT of uneven data length is not possible");
		return false;
	}
	else {
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

		mixfft(cnt,dt,im,tre,tim);

		for(i = 0; i < cnt/2; ++i) {
			dt[i] = tre[i];
			dt[i+cnt/2] = tim[i];
		}

		delete[] im;
		delete[] tre;
		delete[] tim;
		return true;
	}
}

static BL fft_inv_real_any(I cnt,F *dt) 
{
	if(cnt%2 == 1) {
		post("IFFT of uneven data length is not possible");
		return false;
	}
	else {
		float *re,*im,*tim;
		try {
			re = new float[cnt];
			im = new float[cnt];
			tim = new float[cnt];
		}
		catch(...) {
			return false;
		}

		for(I i = 0; i < cnt/2; ++i) {
			re[cnt-i-1] = re[i] = dt[i];
			im[cnt-i-1] = im[i] = dt[cnt/2+i];
		}

		mixfft(cnt,re,im,dt,tim);

		delete[] re;
		delete[] im;
		delete[] tim;
		return true;
	}
}

BL fft_fwd_real_radix2(I cnt,F *dt); 
BL fft_inv_real_radix2(I cnt,F *dt);

static BL fft_fwd_complex_any(I cnt,F *re,F *im) 
{
	if(cnt%2 == 1) {
		post("FFT of uneven data length is not possible");
		return false;
	}
	else {
		float *tre,*tim;
		try {
			tre = new float[cnt];
			tim = new float[cnt];
		}
		catch(...) {
			return false;
		}
		mixfft(cnt,re,im,tre,tim);

		for(I i = 0; i < cnt; ++i) {
			re[i] = tre[i];
			im[i] = tim[i];
		}

		delete[] tre;
		delete[] tim;
		return true;
	}
}

static BL fft_inv_complex_any(I cnt,F *re,F *im) 
{
	if(cnt%2 == 1) {
		post("IFFT of uneven data length is not possible");
		return false;
	}
	else {
		I i;
		for(i = 0; i < cnt; ++i) im[i] = -im[i];
		BL ret = fft_fwd_complex_any(cnt,re,im);
		F nrm = -1./cnt;
		for(i = 0; i < cnt; ++i) im[i] *= nrm;
		return ret;
	}
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


