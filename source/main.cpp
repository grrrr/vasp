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

	VASP_BINARY_SETUP(copy);
	VASP_BINARY_SETUP(ccopy);
	VASP_BINARY_SETUP(mcopy);

	VASP_BINARY_SETUP(add);
	VASP_BINARY_SETUP(cadd);
	VASP_BINARY_SETUP(madd);

	VASP_BINARY_SETUP(sub);
	VASP_BINARY_SETUP(csub);
	VASP_BINARY_SETUP(msub);

	VASP_BINARY_SETUP(mul);
	VASP_BINARY_SETUP(cmul);
	VASP_BINARY_SETUP(mmul);

	VASP_BINARY_SETUP(div);
	VASP_BINARY_SETUP(cdiv);
	VASP_BINARY_SETUP(mdiv);

	VASP_BINARY_SETUP(min)
	VASP_BINARY_SETUP(mmin)

	VASP_BINARY_SETUP(max)
	VASP_BINARY_SETUP(mmax)


	VASP_UNARY_SETUP(sqr) 
	VASP_UNARY_SETUP(ssqr) 
	VASP_UNARY_SETUP(csqr) 
	VASP_UNARY_SETUP(sqrt) 
	VASP_UNARY_SETUP(ssqrt) 
	VASP_BINARY_SETUP(pow) 
	VASP_BINARY_SETUP(root) 

	VASP_UNARY_SETUP(exp) 
	VASP_UNARY_SETUP(log) 

	VASP_UNARY_SETUP(inv) 
	VASP_UNARY_SETUP(cinv) 

	VASP_UNARY_SETUP(abs) 
	VASP_UNARY_SETUP(sign) 
	VASP_UNARY_SETUP(polar) 
	VASP_UNARY_SETUP(cart) 

	VASP_UNARY_SETUP(norm)
	VASP_UNARY_SETUP(cnorm)

	VASP_UNARY_SETUP(cswap)
	VASP_UNARY_SETUP(cconj)

	VASP_BINARY_SETUP(shift)
	VASP_BINARY_SETUP(xshift)
	VASP_BINARY_SETUP(rot)
	VASP_BINARY_SETUP(xrot)
	VASP_UNARY_SETUP(mirr)
	VASP_UNARY_SETUP(xmirr)

	VASP_BINARY_SETUP(osc)
	VASP_BINARY_SETUP(cosc)
	VASP_UNARY_SETUP(noise)
	VASP_UNARY_SETUP(cnoise)

	VASP_UNARY_SETUP(rfft)
	VASP_UNARY_SETUP(rifft)
	VASP_UNARY_SETUP(cfft)
	VASP_UNARY_SETUP(cifft)
}

FLEXT_LIB_SETUP(vasp,lib_setup)

///////////////////////////////////////////////////////////////////////////


