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

///////////////////////////////////////////////////////////////////////////

V vasp_msg::cb_setup(t_class *c)
{
	FLEXT_ADDMETHOD_G(c,"vasp",m_vasp);
	FLEXT_ADDMETHOD_G(c,"set",m_set);

	FLEXT_ADDMETHOD_1(c,"offset",m_offset,F);
	FLEXT_ADDMETHOD_1(c,"length",m_length,F);
	FLEXT_ADDMETHOD_2(c,"part",m_part,F,F);
}

vasp_msg::vasp_msg():
	buf(NULL),
	chn(0),offs(0),len(1<<31)
{
}

vasp_msg::~vasp_msg()
{
	if(buf) delete buf;
}

I vasp_msg::m_set(I argc,t_atom *argv)
{
	I ret = buf->Set(atom_getsymbolarg(0,argc,argv));
	if(ret) {
		m_offset(Offset());
		m_length(Length())
	}
	return ret;
}







///////////////////////////////////////////////////////////////////////////

V vasp_dsp::cb_setup(t_class *c)
{
}


