/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "bpts.h"

Bpts::Bpts(I argc,t_atom *argv):
	cnt(argc/2)
{
	pos = new R[cnt];
	val = new R[cnt];

	R prev = 0;
	BL ok = true;
	for(I i = 0; i < cnt; ++i) {
		pos[i] = flext_base::GetAFloat(argv[i*2]);
		if(pos[i] < prev) ok = false;
		prev = pos[i];
		val[i] = flext_base::GetAFloat(argv[i*2+1]);
	}

	if(!ok) Clear();
}

/*
Bpts::Bpts(const Bpts &s):
	cnt(s.cnt),pos(new R[s.cnt]),val(new R[s.cnt])
{
	for(I i = 0; i < cnt; ++i) pos[i] = s.pos[i],val[i] = s.val[i];
}
*/

Bpts::~Bpts() { Clear(); }

V Bpts::Clear()
{
	if(pos) delete[] pos;
	if(val) delete[] val;
}
