/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include <math.h>

#define PI 3.1415926535897932385


static V d_osc(F *dt,F frq,I cnt) 
{ 
	// how to specify starting phase?
	post("osc: sorry, not yet implemented");
}

static V d_cosc(F *re,F *im,F frq,F,I cnt) 
{ 
	// how to specify starting phase?
	post("cosc: sorry, not yet implemented");
}

Vasp *Vasp::m_osc(const Argument &arg,F ph) 
{ 
	return arg.IsFloat()?fr_nop("osc",arg.GetFloat(),d_osc):NULL; 
}

Vasp *Vasp::m_cosc(const Argument &arg,F ph) 
{ 
	return arg.IsFloat()?fc_nop("cosc",arg.GetFloat(),d_cosc):NULL; 
}


static F rnd() {
    static I val = 307*1319;
    F ret = ((F)((val&0x7fffffff)-0x40000000))*(F)(1.0/0x40000000);
    val = val * 435898247 + 382842987;
	return ret;
}

static V d_noise(F *dt,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) dt[i] = rnd();
}

static V d_cnoise(F *re,F *im,F,F,I cnt) 
{ 
	for(I i = 0; i < cnt; ++i) {
		F amp = rnd();
		F arg = rnd()*(2.*PI);
		re[i] = amp*cos(arg);
		im[i] = amp*sin(arg);
	}
}

Vasp *Vasp::m_noise() { return fr_nop("noise",0,d_noise); }
Vasp *Vasp::m_cnoise() { return fc_nop("cnoise",0,d_cnoise); }


