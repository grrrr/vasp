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

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 102)
#error You need at least flext version 0.1.2 
#endif



class vbuffer:
	public flext_base::buffer
{
	typedef flext_base::buffer parent;

public:
	vbuffer(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);
	vbuffer(const vbuffer &v);
	~vbuffer();

	vbuffer &operator =(const vbuffer &v);
	vbuffer &Set(t_symbol *s = NULL,I chn = 0,I len = -1,I offs = 0);

	I Channel() const { return chn; }
	I Offset() const { return offs; }
	I Length() const { return len; }

protected:
	I chn,offs,len;
};



class vasp 
{
public:

	struct Ref {
		t_symbol *sym;
		I chn;
		I offs; // counted in frames
	};

	vasp();
	vasp(I argc,t_atom *argv);
	vasp(const vasp &v);
	~vasp();

	vasp &operator =(const vasp &v);
	vasp &operator ()(I argc,t_atom *argv /*,BL withvasp = false*/);

	// set used channels to 0
	vasp &Clear() { frames = 0; chns = 0; return *this; }

	// used channels
	I Channels() const { return chns; }

	// length of the vasp (in frames)
	I Frames() const { return frames; }

	BL Ok() const { return ref && Channels() > 0; }
	BL Complex() const { return ref && Channels() >= 2 && ref[1].sym != NULL; }

	// get any channel - test if in range 0..Channels()-1!
	const Ref &Channel(I ix) const { return ref[ix]; }
	Ref &Channel(I ix) { return ref[ix]; }

	// get real part - be sure that Ok!
	const Ref &Real() const { return Channel(0); }
	Ref &Real() { return Channel(0); }

	// get imaginary part - be sure that Complex!
	const Ref &Imag() const { return Channel(1); }
	Ref &Imag() { return Channel(1); }

	// get buffer associated to a channel
	vbuffer *Buffer(I ix) const;

	// Real/Complex
	vbuffer *ReBuffer() const { return Buffer(0); }
	vbuffer *ImBuffer() const { return Buffer(1); }

	// prepare and reference t_atom list for output
	vasp &MakeList(BL withvasp = true);
	I Atoms() const { return atoms; }
	t_atom *AtomList() { return atomlist; }

	// gensym("vasp")
	static t_symbol *const vaspsym;

protected:
	I frames; // length counted in frames
	I chns; // used channels
	I refs; // allocated channels
	Ref *ref;

	I atoms; // elements of list
	t_atom *atomlist;
};




class vasp_msg:
	public flext_base
{
	FLEXT_HEADER(vasp_msg,flext_base)

	enum xs_unit {
		xsu__ = -1,  // don't change
		xsu_sample = 0,xsu_buffer,xsu_ms,xsu_s
	};	

protected:
	vasp_msg();
	virtual ~vasp_msg();

	virtual V m_bang();						// output current vasp

	virtual V m_vasp(I argc,t_atom *argv); // trigger
	virtual I m_set(I argc,t_atom *argv) = 0;  // non trigger

	virtual V m_unit(xs_unit u);  

	vasp ref;

private:
	
	xs_unit unit;

	FLEXT_CALLBACK(m_bang)
	FLEXT_CALLBACK_G(m_vasp)
	FLEXT_CALLBACK_G(m_set)

	FLEXT_CALLBACK_E(m_unit,xs_unit)
};


class vasp_dsp:
	public flext_dsp
{
	FLEXT_HEADER(vasp_dsp,flext_dsp)

protected:

};



#endif
