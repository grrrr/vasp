/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#ifndef __VASP_VBUFFER_H
#define __VASP_VBUFFER_H

#include "main.h"

/*
class VBuffer:
	public flext_base::buffer
{
	typedef flext_base::buffer parent;

public:
	VBuffer(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);
	VBuffer(const VBuffer &v);
	~VBuffer();

	VBuffer &operator =(const VBuffer &v);
	VBuffer &Set(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);

	I Channel() const { return chn; }
	I Offset() const { return offs; }
	I Length() const { return len; }

	S *Pointer() { return Data()+Offset()*Channels()+Channel(); }

protected:
	I chn,offs,len;
};
*/

class VSym
{
public:
	VSym(t_symbol *s = NULL): sym(s) { Inc(); }
	VSym(VSym &s): sym(s.sym) { Inc(); }
	~VSym() { Dec(); }
	
	V Clear() { Dec(); sym = NULL; }
	
	VSym &operator =(VSym &s) { Clear(); sym = s.sym; Inc(); return *this; }
	
	t_symbol *Symbol() { return sym; }
	const t_symbol *Symbol() const { return sym; }
	const C *Name() const { return flext_base::GetString(Symbol()); }
	
protected:
	V Inc(); // { if(sym) BufLib::IncRef(s); }
	V Dec(); // { if(sym) BufLib::DecRef(s); }

	t_symbol *sym;
};

class VBuffer
{
public:
	virtual ~VBuffer() {}

	virtual BL Ok() const = 0;
	virtual I Frames() const = 0;
	virtual V Frames(I fr,BL keep) = 0;

	virtual I Channels() const = 0;
	virtual S *Data() = 0;

	virtual V Dirty() = 0;

/*
	virtual V IncRef() = 0;
	virtual V DecRef() = 0;
*/

	S *Pointer() { return Data()+Offset()*Channels()+Channel(); }

	virtual VSym Symbol() const = 0;
	const C *Name() const { return Symbol().Name(); }

	I Channel() const { return chn; }
	V Channel(I c) { chn = c; }

	I Offset() const { return offs; }
	V Offset(I o) { offs = o; }

	I Length() const { return len; }
	V Length(I l) { len = l; }

protected:
	VBuffer(I c = 0,I l = 0,I o = 0): chn(c),offs(o),len(l) {}

	I chn,offs,len;
};


class SysBuf:
	public VBuffer
{
public:
	SysBuf(VSym &s,I chn = 0,I len = -1,I offs = 0) { Set(s,chn,len,offs); }
	virtual ~SysBuf() {}

	virtual BL Ok() const { return buf.Ok(); }
	virtual V Dirty() { buf.Dirty(true); }

/*
	virtual V IncRef() {};
	virtual V DecRef() {};
*/

	virtual VSym Symbol() const { return buf.Symbol(); }

	SysBuf &Set(VSym &s,I chn = 0,I len = -1,I offs = 0);

	virtual I Frames() const { return buf.Frames(); }
	virtual V Frames(I fr,BL keep) { buf.Frames(fr,keep); }

	virtual I Channels() const { return buf.Channels(); }
	virtual S *Data() { return buf.Data(); }

protected:
	flext_base::buffer buf;
};


class BufEntry;

class ImmBuf:
	public VBuffer
{
public:
	ImmBuf(I len);
	ImmBuf(BufEntry *e,I len = -1,I offs = 0);
	virtual ~ImmBuf();

	virtual BL Ok() const { return entry != NULL; }
	virtual V Dirty() {}

/*
	virtual V IncRef();
	virtual V DecRef();
*/

	virtual VSym Symbol() const;

//	ImmBuf &Set(t_symbol *s,I chn = 0,I len = -1,I offs = 0);

	virtual I Frames() const;
	virtual V Frames(I fr,BL keep);

	virtual I Channels() const { return 1; }
	virtual S *Data();

protected:
	BufEntry *entry;
};


#endif
