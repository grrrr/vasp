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

static F arg(F re,F im)
{
	if(re) 
		return atan(im/re)+(re < 0?PI:0);
	else
		if(im || re) return im > 0?PI/2:-PI/2;
		else return 0;
}

static F abs(F re,F im) { return re*re+im*im; }


static BL d_osc(I cnt,F *dt,I str,F *,I,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,dt += str) *dt = cos(ph);
	return true;
}

static BL d_cosc(I cnt,F *re,I rstr,F *im,I istr,F rfrq,F ifrq) 
{ 
	// frq and phase defined by complex frequency
	F phinc = abs(rfrq,ifrq),ph = arg(rfrq,ifrq); 
	for(I i = 0; i < cnt; ++i,ph += phinc,re += rstr,im += istr) *re = cos(ph),*im = sin(ph);
	return true;
}

Vasp *Vasp::m_osc(const Argument &arg) 
{ 
	if(arg.IsFloat()) 
		return fc_nop("osc",arg.GetFloat(),d_osc);
	else if(arg.IsComplex()) 
		return fc_nop("osc",arg.GetComplex(),d_osc);
	else
		return NULL; 
}

Vasp *Vasp::m_cosc(const Argument &arg) 
{ 
	if(arg.IsFloat()) 
		return fc_nop("cosc",arg.GetFloat(),d_osc);
	else if(arg.IsComplex()) 
		return fc_nop("cosc",arg.GetComplex(),d_osc);
	else
		return NULL; 
}


static F rnd() 
{
    static I val = 307*1319;
    F ret = ((F)((val&0x7fffffff)-0x40000000))*(F)(1.0/0x40000000);
    val = val * 435898247 + 382842987;
	return ret;
}

static BL d_noise(I cnt,F *dt,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt = rnd();
	return true;
}

static BL d_cnoise(I cnt,F *re,I rstr,F *im,I istr,F,F) 
{ 
	for(I i = 0; i < cnt; ++i,re += rstr,im += istr) {
		F amp = rnd();
		F arg = rnd()*(2.*PI);
		*re = amp*cos(arg);
		*im = amp*sin(arg);
	}
	return true;
}

Vasp *Vasp::m_noise() { return fr_nop("noise",0,d_noise); }
Vasp *Vasp::m_cnoise() { return fc_nop("cnoise",0,d_cnoise); }


static BL d_bevelup(I cnt,F *dt,I str,F) 
{ 
	for(I i = 0; i < cnt; ++i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

static BL d_beveldn(I cnt,F *dt,I str,F) 
{ 
	for(I i = cnt-1; i >= 0; -i,dt += str) *dt = (i+.5)/cnt;
	return true;
}

Vasp *Vasp::m_bevelup() { return fr_nop("bevel",0,d_bevelup); }
Vasp *Vasp::m_beveldn() { return fr_nop("bevel-",0,d_beveldn); }

