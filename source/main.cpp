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
FLEXT_EXT V vasp_copy_setup();
FLEXT_EXT V vasp_ccopy_setup();
FLEXT_EXT V vasp_mcopy_setup();


FLEXT_EXT V vasp_setup()
{
	post("VASP modular %s - vector assembling signal processor, (C)2002 Thomas Grill",VASP_VERSION);
	post("");

	// call the objects' setup routines

	vasp_v_setup();

	vasp_copy_setup();
	vasp_ccopy_setup();
	vasp_mcopy_setup();
}
}
#endif



///////////////////////////////////////////////////////////////////////////


