/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_H
#define __VASP_H

#define VASP_VERSION "0.0.1"


#include <flext.h>

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 101)
#error You need at least flext version 0.1.1 
#endif


class vasp_msg:
	public flext_base
{
	FLEXT_HEADER(vasp_msg,flext_base)

protected:


};


class vasp_dsp:
	public flext_dsp
{
	FLEXT_HEADER(vasp_dsp,flext_dsp)

protected:


};



#endif
