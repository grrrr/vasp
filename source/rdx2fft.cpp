#include <math.h>

#define PI 3.1415926535897932384f

/* calculate array size from spectrum size */

int fft_time_domain_size(int freq_domain_size)
{
  int size;
  size=2*(freq_domain_size-1);
  return size;
}

/* calculate spectrum size from array size */

int fft_freq_domain_size(int time_domain_size)
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

bool fft_bidir_complex_radix2(int size,float *real,float *imag,int direct)
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


/*
  // for forward transform divide by size 
  if(direct<0)
  {
    for(i=0;i<size;i++)
    {
      real[i]/=(float)size;
      imag[i]/=(float)size;
    }
  }
*/

	const float nf = (float)(1./sqrt(size));
	for(i=0;i<size;i++)
	{
	  real[i] *= nf;
	  imag[i] *= nf;
	}
	return true;
}

/* calculate forward fourier transform of complex data radix 2 */

bool fft_fwd_complex_radix2(int size,float *real,float *imag)
{
  return fft_bidir_complex_radix2(size,real,imag,-1);
}

/* calculate inverse fourier transform of complex data radix 2 */

bool fft_inv_complex_radix2(int size,float *real,float *imag)
{
  return fft_bidir_complex_radix2(size,real,imag,1);
}

/* calculate forward fourier transform of real data radix 2 */

bool fft_fwd_real_radix2(int size,float *real)
{
  register int i;
  float *imag;
//  try {
	imag = new float[size]; // should be aligned
/*
  }
  catch(...) {
	  return false;
  }
*/
	if(!imag) return false;

  for(i=0;i<size;i++) imag[i]=(float)0;
  fft_fwd_complex_radix2(size,real,imag);
  const int n2 = size/2;
  const float nf = (float)sqrt(size);
  for(i = 1; i < n2; ++i) {
//	  real[i] *= nf;
	  real[n2+i] = imag[i]; //*nf;
  }
  delete[] imag;
  return true;
}

/*
bool fft_fwd_real_radix2(int size,float *real,float *imag)
{
  register int i;
  const float nf = sqrt(n);
  for(i=0;i<size;i++) {
	  real[i] *= nf;
	  imag[i] = 0;
  }
  return ret = fft_fwd_complex_radix2(size,real,imag);
}
*/

/* calculate inverse fourier transform of real data radix 2 */

bool fft_inv_real_radix2(int size,float *real)
{
  float *imag;
//  try {
	imag = new float[size]; // should be aligned
/*
	}
  catch(...) {
	  return false;
  }
*/
	if(!imag) return false;

  register int i;
  const int n2 = size/2;
  const float nf = (float)(1./sqrt(size));
  for(i = 0; i < n2; ++i) {
//	  real[i] *= nf;
	  imag[i] = real[n2+i]; //*nf;
  }

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
  return true;
}

/* calculate hilbert transform of real data radix 2 */
/*
void fft_hilbert_real_radix2(int size,float *real,float *imag)
{
  int i,spectrumsize;

  // calculate forward fourier transform of real data 

  fft_fwd_real_radix2(size,real,imag);

  // determine spectrum size 

  spectrumsize=fft_freq_domain_size(size);

  // zero negative frequency half of spectrum 

  for(i=spectrumsize;i<size;i++)
  {
    real[i]=(float)0;
    imag[i]=(float)0;
  }

  // double positive frequency half of spectrum 

  for(i=1;i<spectrumsize;i++)
  {
    real[i]*=(float)2;
    imag[i]*=(float)2;
  }

  // calculate inverse fourier transform of complex data 

  fft_inv_complex_radix2(size,real,imag);
}
*/




