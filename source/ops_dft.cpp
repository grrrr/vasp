/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////

#define PI 3.141592653589793238

inline D squ(D x) { return x*x; }
inline F squ(F x) { return x*x; }

/* calculate array size from spectrum size */

I fft_time_domain_size(I freq_domain_size)
{
  int size;
  size=2*(freq_domain_size-1);
  return size;
}

/* calculate spectrum size from array size */

I fft_freq_domain_size(I time_domain_size)
{
  int size;
  size=time_domain_size/2+1;
  return size;
}

//////////////////////////////////////////////////////////////////////////

/* calculate bidirectional fourier transform of complex data radix 2 */
/* adapted from subroutine FOUREA listed in                          */
/* Programs for Digital Signal Processing                            */
/* edited by Digital Signal Processing Committee                     */
/* IEEE Acoustics Speech and Signal Processing Committee             */
/* Chapter 1 Section 1.1 Page 1.1-4,5                                */
/* direct -1 forward +1 reverse                                      */

void fft_bidir_complex_radix2(int size,float *real,float *imag,int direct)
{
  int i,j,m,mmax,istep;
  float c,s,treal,timag,theta; 

  /* compute transform */

  j=1;
  for(i=1;i<=size;i++)
  {
    if(i<j)
    {
      treal=real[j-1];
      timag=imag[j-1];
      real[j-1]=real[i-1];
      imag[j-1]=imag[i-1];
      real[i-1]=treal;
      imag[i-1]=timag;
    }
    m=size/2;
    while(j>m)
    {
      j-=m;
      m=(m+1)/2;
    }
    j+=m;
  }
  mmax=1;
  while(size>mmax)
  {
    istep=2*mmax;
    for(m=1;m<=mmax;m++)
    {
      theta=PI*(float)direct*(float)(m-1)/(float)mmax;
      c=(float)cos(theta);
      s=(float)sin(theta);
      for(i=m;i<=size;i+=istep)
      {
	j=i+mmax;
	treal=real[j-1]*c-imag[j-1]*s;
	timag=imag[j-1]*c+real[j-1]*s;
	real[j-1]=real[i-1]-treal;
	imag[j-1]=imag[i-1]-timag;
	real[i-1]+=treal;
	imag[i-1]+=timag;
      }
    }
    mmax=istep;
  }

  /* for forward transform divide by size */

  if(direct<0)
  {
    for(i=0;i<size;i++)
    {
      real[i]/=(float)size;
      imag[i]/=(float)size;
    }
  }
}

/* calculate forward fourier transform of complex data radix 2 */

inline fft_fwd_complex_radix2(int size,float *real,float *imag)
{
  fft_bidir_complex_radix2(size,real,imag,-1);
}

/* calculate inverse fourier transform of complex data radix 2 */

inline fft_inv_complex_radix2(int size,float *real,float *imag)
{
  fft_bidir_complex_radix2(size,real,imag,1);
}

/* calculate forward fourier transform of real data radix 2 */

void fft_fwd_real_radix2(int size,float *real)
{
  register int i;
  float *imag = new float[size]; // should be aligned
  for(i=0;i<size;i++) imag[i]=(float)0;
  fft_fwd_complex_radix2(size,real,imag);
  delete[] imag;
}

void fft_fwd_real_radix2(int size,float *real,float *imag)
{
  register int i;
  for(i=0;i<size;i++) imag[i]=(float)0;
  fft_fwd_complex_radix2(size,real,imag);
}

/* calculate inverse fourier transform of real data radix 2 */

void fft_inv_real_radix2(int size,float *real)
{
  float *imag = new float[size]; // should be aligned
  register int i;
  int j,spectrumsize;
  spectrumsize=fft_freq_domain_size(size);
  for(i=2;i<=spectrumsize;i++)
  {
    j=size+2-i;
    real[j-1]=real[i-1];

	// imag = f(real)!!
    imag[j-1]= -imag[i-1];
  }
  fft_inv_complex_radix2(size,real,imag);
  delete[] imag;
}

/* calculate hilbert transform of real data radix 2 */

void fft_hilbert_real_radix2(int size,float *real,float *imag)
{
  int i,spectrumsize;

  /* calculate forward fourier transform of real data */

  fft_fwd_real_radix2(size,real,imag);

  /* determine spectrum size */

  spectrumsize=fft_freq_domain_size(size);

  /* zero negative frequency half of spectrum */

  for(i=spectrumsize;i<size;i++)
  {
    real[i]=(float)0;
    imag[i]=(float)0;
  }

  /* double positive frequency half of spectrum */

  for(i=1;i<spectrumsize;i++)
  {
    real[i]*=(float)2;
    imag[i]*=(float)2;
  }

  /* calculate inverse fourier transform of complex data */

  fft_inv_complex_radix2(size,real,imag);
}

///////////////////////////////////////////////////////////////

/* calculate bidirectional complex fourier transform   */
/* for any of data points                              */
/* adapted from Holmes,J.T.,Naval Ordnance Test Lab    */
/* Report NOTLR 72-299 October 1972 ADSATIS AD-754 393 */

void fft_bidir_complex_any(int size,float *real,float *imag,int dirn)
{
  int i,j,k,l,f,a,n,c,r,w,q,b,p,s,d,t,*fact;
  float sign,expo,theta,sintheta,costheta;
  float temp,*work;
  fact=new int[256];
//  work = (F *)flext_obj::NewAligned(sizeof(*work)*size*2);
  work = new F[size*2];
  if(fact==NULL||work==NULL) {
	  error("Insufficient memory for fft");
	  return;
  }
  if(dirn>0) sign=(float)1;
  else sign=(float)(-1);
  f=0;
  a=1;
  c=size;
  do
  {
    f++;
    n=c;
    expo=sign*PI*((float)2/(float)n);
    r=2;
    if(f>=2) r=fact[f-2];
    while((n%r)!=0) r++;
    fact[f-1]=r;
    c=n/r;
    for(l=0;l<a;l++)
    {
      b=l*n;
      for(j=0;j<c;j++)
      {
	for(i=0;i<r;i++)
	{
	  work[2*i]=(float)0;
	  work[2*i+1]=(float)0;
	  p=b+j;
	  for(k=0;k<r;k++)
	  {
	    w=(i*(k*c+j))%n;
	    theta=expo*(float)w;
	    sintheta=(float)sin(theta);
	    costheta=(float)cos(theta);
	    work[2*i]+=real[p]*costheta-imag[p]*sintheta;
	    work[2*i+1]+=real[p]*sintheta+imag[p]*costheta;
	    p+=c;
	  }
	}
	for(i=0;i<r;i++)
	{
	  p=b+i*c+j;
	  real[p]=work[2*i];
	  imag[p]=work[2*i+1];
	}
      }
    }
    a*=r;
  } while(c!=1);

	if(dirn<0) {
		for(i=0;i<size;i++) {
				real[i] /= (float)size;
				imag[i] /= (float)size;
		}
	}

	for(d=0;d<size;d++) {
		s=d;
		do {
			n=size;
			q=s;
			s=0;
			for(i=0;i<f;i++) {
				t = q/fact[i];
				r = q-fact[i]*t;
				q = t;
				n /= fact[i];
				s += n*r;
			}
		} while(s < d);

		temp = real[d];
		real[d] = real[s];
		real[s] = temp;
		temp = imag[d];
		imag[d] = imag[s];
		imag[s] = temp;
	}

	delete[] fact;
//	flext_obj::FreeAligned(work);
	delete[] work;
}

/* calculate forward transform of complex data for any number of points */

inline fft_fwd_complex_any(int size,float *real,float *imag)
{
  fft_bidir_complex_any(size,real,imag,-1);
}

/* calculate inverse transform of complex data for any number of points */

inline fft_inv_complex_any(int size,float *real,float *imag)
{
  fft_bidir_complex_any(size,real,imag,1);
}

/* calculate forward transform of real data for any number of points */

void fft_fwd_real_any(int size,float *real)
{
  int i;
  float *imag = new float[size]; // should be aligned
  for(i=0;i<size;i++) imag[i]=(float)0;
  fft_fwd_complex_any(size,real,imag);
  delete[] imag;
}

/* calculate inverse transform of real data for any number of points */

void fft_inv_real_any(int size,float *real)
{
  float *imag = new float[size]; // should be aligned
  int i,j,spectrumsize;
  spectrumsize=fft_freq_domain_size(size);
  for(i=1;i<spectrumsize;i++)
  {
    j=size-i;
    real[j]=real[i];
	// imag = f(real)
    imag[j]= -imag[i];
  }
  fft_inv_complex_any(size,real,imag);
  delete[] imag;
}

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

///////////////////////////////////////////////////////////////

static V d_rfft(F *dt,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1)
			fft_fwd_real_radix2(cnt,dt);
		else
			fft_fwd_real_any(cnt,dt);
}

static V d_rifft(F *dt,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1)
			fft_inv_real_radix2(cnt,dt);
		else
			fft_inv_real_any(cnt,dt);
}

Vasp *Vasp::m_rfft() { return fr_nop("rfft",0,d_rfft); }
Vasp *Vasp::m_rifft() { return fr_nop("rifft",0,d_rifft); }


static V d_cfft(F *re,F *im,F,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1)
			fft_fwd_complex_radix2(cnt,re,im);
		else
			fft_fwd_complex_any(cnt,re,im);
}

static V d_cifft(F *re,F *im,F,F,I cnt) 
{ 
	if(cnt)
		if(radix2(cnt) >= 1)
			fft_inv_complex_radix2(cnt,re,im);
		else
			fft_inv_complex_any(cnt,re,im);
}

Vasp *Vasp::m_cfft() { return fc_nop("cfft",0,d_cfft); }
Vasp *Vasp::m_cifft() { return fc_nop("cifft",0,d_cifft); } 


