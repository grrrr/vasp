/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


#ifdef PD
// Initialization for pd xsample library

extern "C" {
// base classes
FLEXT_EXT V vasp_v_setup();
FLEXT_EXT V vasp_b_copy_setup();
FLEXT_EXT V vasp_b_ccopy_setup();
FLEXT_EXT V vasp_b_mcopy_setup();

FLEXT_EXT V vasp_b_add_setup();
FLEXT_EXT V vasp_b_cadd_setup();
FLEXT_EXT V vasp_b_madd_setup();

FLEXT_EXT V vasp_b_sub_setup();
FLEXT_EXT V vasp_b_csub_setup();
FLEXT_EXT V vasp_b_msub_setup();

FLEXT_EXT V vasp_b_mul_setup();
FLEXT_EXT V vasp_b_cmul_setup();
FLEXT_EXT V vasp_b_mmul_setup();

FLEXT_EXT V vasp_b_div_setup();
FLEXT_EXT V vasp_b_cdiv_setup();
FLEXT_EXT V vasp_b_mdiv_setup();



FLEXT_EXT V vasp_setup()
{
	post("VASP modular %s - vector assembling signal processor, (C)2002 Thomas Grill",VASP_VERSION);
	post("");

	// call the objects' setup routines

	vasp_v_setup();

	vasp_b_copy_setup();
	vasp_b_ccopy_setup();
	vasp_b_mcopy_setup();

	vasp_b_add_setup();
	vasp_b_cadd_setup();
	vasp_b_madd_setup();

	vasp_b_sub_setup();
	vasp_b_csub_setup();
	vasp_b_msub_setup();

	vasp_b_mul_setup();
	vasp_b_cmul_setup();
	vasp_b_mmul_setup();

	vasp_b_div_setup();
	vasp_b_cdiv_setup();
	vasp_b_mdiv_setup();
}
}
#endif

///////////////////////////////////////////////////////////////////////////


