/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "buflib.h"

VBuffer *BufLib::Get(t_symbol *s,I chn,I len,I offs)
{
	return new SysBuf(s,chn,len,offs);
}
