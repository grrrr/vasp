/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file buflib.h
	\brief Routines for buffer management
*/

#ifndef __VASP_BUFLIB_H
#define __VASP_BUFLIB_H

#include "classes.h"
#include "vbuffer.h"

namespace BufLib
{
	VBuffer *Get(t_symbol *s,I chn = 0,I len = -1,I offs = 0);
};



#endif
