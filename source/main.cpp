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

// utils
FLEXT_EXT V vasp_prepend_setup();


FLEXT_EXT V vasp_setup()
{
	post("VASP - vector assembling signal processor, (C)2002 Thomas Grill");
	post("");

	// call the objects' setup routines

	vasp_v_setup();

	vasp_prepend_setup();
}
}
#endif



V vasp_msg::cb_setup(t_class *c)
{
}


V vasp_dsp::cb_setup(t_class *c)
{
}


