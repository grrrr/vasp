#include "vbuffer.h"
//#include <math.h>


///////////////////////////////////////////////////////////////////////////
// SysBuf class
///////////////////////////////////////////////////////////////////////////

/*
SysBuf::SysBuf(t_symbol *s,I c,I l,I o)
{	
	Set(s,c,l,o); 
}

VBuffer::VBuffer(const VBuffer &v):
	chn(0),len(0),offs(0) 
{ 
	operator =(v); 
}

SysBuf::~SysBuf() {}

VBuffer &VBuffer::operator =(const VBuffer &v)
{
	return Set(v.Symbol(),v.Channel(),v.Length(),v.Offset());
}
*/

SysBuf &SysBuf::Set(t_symbol *s,I c,I l,I o)
{
	buf.Set(s);

	chn = c;
	if(chn > Channels()) chn = Channels()-1; // simply correct the channel??
	offs = o;
	if(offs > Frames()) offs = Frames();
	len = l >= 0?l:Frames();
	if(offs+len > Frames()) len = Frames()-offs;

	return *this;
}


