#include "vbuffer.h"
#include "buflib.h"

V VSymbol::Inc() { if(sym) BufLib::IncRef(sym); }
V VSymbol::Dec() { if(sym) BufLib::DecRef(sym); }

///////////////////////////////////////////////////////////////////////////
// SysBuf class
///////////////////////////////////////////////////////////////////////////

SysBuf &SysBuf::Set(const VSymbol &s,I c,I l,I o)
{
	buf.Set(s.Symbol());

	chn = c;
	if(chn > Channels()) chn = Channels()-1; // simply correct the channel??
	offs = o;
	if(offs > Frames()) offs = Frames();
	len = l >= 0?l:Frames();
	if(offs+len > Frames()) len = Frames()-offs;

	return *this;
}


