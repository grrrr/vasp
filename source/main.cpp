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
#ifdef _DEBUG
	post("DEBUG MODE");
#endif
	post("");

	// call the objects' setup routines

	VASP_SETUP(v); // vasp
	VASP_SETUP(m); // vasp.m
	VASP_SETUP(n); // vasp.n
	VASP_SETUP(qn); // vasp.n?

	VASP_SETUP(chk);  // vasp.chk
	VASP_SETUP(update);  // vasp.upd
	VASP_SETUP(sync);  // vasp.sync

	VASP_SETUP(offs);  // vasp.offs
	VASP_SETUP(doffs); // vasp.offs+
	VASP_SETUP(qoffs);  // vasp.offs?
	VASP_SETUP(frames);  // vasp.frames
	VASP_SETUP(dframes);  // vasp.frames+
	VASP_SETUP(qframes);  // vasp.frames?

	VASP_SETUP(split);
	VASP_SETUP(join);
	VASP_SETUP(spit);
	VASP_SETUP(gather);
	VASP_SETUP(part);

	VASP_SETUP(q);
	VASP_SETUP(qq);
	VASP_SETUP(i);

//	VASP__SETUP(copy);
//	VASP__SETUP(ccopy);
	VASP__SETUP(set);
	VASP__SETUP(cset);

	VASP__SETUP(add);
	VASP__SETUP(cadd);
	VASP__SETUP(sub);
	VASP__SETUP(csub);
	VASP__SETUP(subr);
	VASP__SETUP(csubr);
	VASP__SETUP(mul);
	VASP__SETUP(cmul);
	VASP__SETUP(div);
	VASP__SETUP(cdiv);
	VASP__SETUP(divr);
	VASP__SETUP(cdivr);
	VASP__SETUP(mod);

	VASP__SETUP(min)
	VASP__SETUP(rmin)
	VASP__SETUP(max)
	VASP__SETUP(rmax)

	VASP__SETUP(lwr)
	VASP__SETUP(gtr)
	VASP__SETUP(leq)
	VASP__SETUP(geq)
	VASP__SETUP(equ)
	VASP__SETUP(neq)

	VASP__SETUP(int) 
	VASP__SETUP(dif) 
	VASP_SETUP(peaks) 
	VASP_SETUP(valleys) 
	VASP_SETUP(rpeaks) 
	VASP_SETUP(rvalleys) 

	VASP__SETUP(minmax) 

	VASP__SETUP(sqr) 
	VASP__SETUP(ssqr) 
	VASP__SETUP(csqr) 
	VASP__SETUP(sqrt) 
	VASP__SETUP(ssqrt) 
	VASP__SETUP(pow) 
	VASP__SETUP(cpowi) 
	VASP__SETUP(rpow);
	VASP__SETUP(radd);

	VASP__SETUP(exp) 
	VASP__SETUP(log) 

//	VASP__SETUP(inv) 
//	VASP__SETUP(cinv) 

	VASP__SETUP(abs) 
	VASP__SETUP(sign) 
	VASP__SETUP(polar) 
	VASP__SETUP(rect) 

//	VASP_SETUP(opt)
//	VASP_SETUP(ropt)
	VASP__SETUP(gate);
	VASP__SETUP(rgate);
//	VASP__SETUP(igate);
//	VASP__SETUP(rigate);

	VASP_SETUP(qmin)
	VASP_SETUP(qmax)
	VASP_SETUP(qamin)
	VASP_SETUP(qamax)
	VASP_SETUP(qrmin)
	VASP_SETUP(qrmax)

	VASP__SETUP(cnorm)
//	VASP__SETUP(cswap)
	VASP__SETUP(cconj)

	VASP__SETUP(shift)
	VASP__SETUP(xshift)
	VASP__SETUP(rot)
	VASP__SETUP(xrot)
	VASP__SETUP(mirr)
	VASP__SETUP(xmirr)

	VASP__SETUP(osc)
	VASP__SETUP(mosc)
	VASP__SETUP(cosc)
	VASP__SETUP(mcosc)
	VASP__SETUP(phasor)
	VASP__SETUP(mphasor)
	VASP__SETUP(noise)
	VASP__SETUP(cnoise)
	VASP__SETUP(bevelup)
	VASP__SETUP(beveldn)
	VASP__SETUP(mbevelup)
	VASP__SETUP(mbeveldn)
	VASP__SETUP(window)
	VASP__SETUP(mwindow)

	VASP__SETUP(flp)
	VASP__SETUP(fhp)

	VASP__SETUP(tilt)
	VASP__SETUP(xtilt)

	VASP__SETUP(rfft)
	VASP__SETUP(rifft)
	VASP__SETUP(cfft)
	VASP__SETUP(cifft)
}

FLEXT_LIB_SETUP(vasp,lib_setup)

///////////////////////////////////////////////////////////////////////////


