/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


V lib_setup()
{
	post("VASP modular %s - vector assembling signal processor, (C)2002 Thomas Grill",VASP_VERSION);
	post("");

	// call the objects' setup routines

	VASP_SETUP(v);
	VASP_SETUP(m);
	VASP_SETUP(n);
	VASP_SETUP(qn);

	VASP_SETUP(chk);
	VASP_SETUP(update);
	VASP_SETUP(sync);

	VASP_SETUP(offs);
	VASP_SETUP(doffs);
	VASP_SETUP(qoffs);
	VASP_SETUP(frames);
	VASP_SETUP(dframes);
	VASP_SETUP(qframes);

	VASP_SETUP(split);
	VASP_SETUP(join);
	VASP_SETUP(spit);
	VASP_SETUP(gather);
	VASP_SETUP(part);

	VASP_SETUP(q);
	VASP_SETUP(i);

//	VASP_UNARY_SETUP(copy);
//	VASP_UNARY_SETUP(ccopy);
	VASP_BINARY_SETUP(set);
	VASP_BINARY_SETUP(cset);

	VASP_BINARY_SETUP(add);
	VASP_BINARY_SETUP(cadd);
	VASP_BINARY_SETUP(sub);
	VASP_BINARY_SETUP(csub);
	VASP_BINARY_SETUP(mul);
	VASP_BINARY_SETUP(cmul);
	VASP_BINARY_SETUP(div);
	VASP_BINARY_SETUP(cdiv);
	VASP_BINARY_SETUP(divr);
	VASP_BINARY_SETUP(cdivr);
	VASP_BINARY_SETUP(mod);

	VASP_BINARY_SETUP(min)
	VASP_BINARY_SETUP(cmin)
	VASP_BINARY_SETUP(max)
	VASP_BINARY_SETUP(cmax)

	VASP_BINARY_SETUP(lwr)
	VASP_BINARY_SETUP(gtr)
	VASP_BINARY_SETUP(leq)
	VASP_BINARY_SETUP(geq)
	VASP_BINARY_SETUP(equ)
	VASP_BINARY_SETUP(neq)

	VASP_UNARY_SETUP(int) 
	VASP_UNARY_SETUP(dif) 
	VASP_BINARY_SETUP(peaks) 
	VASP_BINARY_SETUP(valleys) 

	VASP_UNARY_SETUP(minmax) 

	VASP_UNARY_SETUP(sqr) 
	VASP_UNARY_SETUP(ssqr) 
	VASP_UNARY_SETUP(csqr) 
	VASP_UNARY_SETUP(sqrt) 
	VASP_UNARY_SETUP(ssqrt) 
	VASP_BINARY_SETUP(pow) 
	VASP_BINARY_SETUP(cpowi) 
	VASP_BINARY_SETUP(rpow);

	VASP_UNARY_SETUP(exp) 
	VASP_UNARY_SETUP(log) 

	VASP_UNARY_SETUP(inv) 
	VASP_UNARY_SETUP(cinv) 

	VASP_UNARY_SETUP(abs) 
	VASP_UNARY_SETUP(sign) 
	VASP_UNARY_SETUP(polar) 
	VASP_UNARY_SETUP(cart) 

	VASP_UNARY_SETUP(opt)
	VASP_UNARY_SETUP(copt)
	VASP_UNARY_SETUP(cnorm)

	VASP_UNARY_SETUP(cswap)
	VASP_UNARY_SETUP(cconj)

	VASP_BINARY_SETUP(shift)
	VASP_BINARY_SETUP(xshift)
	VASP_BINARY_SETUP(rot)
	VASP_BINARY_SETUP(xrot)
	VASP_UNARY_SETUP(mirr)
	VASP_UNARY_SETUP(xmirr)

	VASP_ANYOP_SETUP(osc)
	VASP_ANYOP_SETUP(mosc)
	VASP_ANYOP_SETUP(cosc)
	VASP_ANYOP_SETUP(mcosc)
	VASP_ANYOP_SETUP(phasor)
	VASP_ANYOP_SETUP(mphasor)
	VASP_UNARY_SETUP(noise)
	VASP_UNARY_SETUP(cnoise)
	VASP_UNARY_SETUP(bevelup)
	VASP_UNARY_SETUP(beveldn)
	VASP_UNARY_SETUP(mbevelup)
	VASP_UNARY_SETUP(mbeveldn)
	VASP_BINARY_SETUP(window)
	VASP_BINARY_SETUP(mwindow)

	VASP_ANYOP_SETUP(flp)
	VASP_ANYOP_SETUP(fhp)

	VASP_ANYOP_SETUP(tilt)
	VASP_ANYOP_SETUP(xtilt)

	VASP_UNARY_SETUP(rfft)
	VASP_UNARY_SETUP(rifft)
	VASP_UNARY_SETUP(cfft)
	VASP_UNARY_SETUP(cifft)
}

FLEXT_LIB_SETUP(vasp,lib_setup)

///////////////////////////////////////////////////////////////////////////


