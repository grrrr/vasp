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


class BufEntry
{
public:
	BufEntry(t_symbol *s,I fr);
	~BufEntry();
/*
	V IncRef();
	V DecRef();
*/
	VSym sym;
	I refcnt,tick;
	BufEntry *nxt;

	UL magic;
	I alloc,len;
	S *data;
};


namespace BufLib
{
	VBuffer *Get(VSym &s,I chn = 0,I len = -1,I offs = 0);

	BufEntry *NewImm(I fr);
/*
	V IncRef(t_symbol *s);
	V DecRef(t_symbol *s);
*/
	BufEntry *Resize(BufEntry *e,I fr,BL keep = false); 
}



#endif
