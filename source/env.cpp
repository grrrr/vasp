/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "bpts.h"

Bpts::Bpts(I argc,t_atom *argv):
	cnt(argc),pts(new R[argc])
{
	for(I i = 0; i < cnt; ++i)
		pts[i] = GetAFloat(argv[i]);
}

Bpts::~Bpts()
{
	if(pts) delete[] pts;
}
