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
	vasp_msg();
	virtual ~vasp_msg();

	virtual V m_vasp(I argc,t_atom *argv); // trigger
	virtual I m_set(I argc,t_atom *argv);  // non trigger

	virtual V m_offset(F offs);
	virtual V m_length(F len);
	virtual V m_part(F offs,F len);

	I Channel() const { return chn; }
	I Offset() const { return offs; }
	I Length() const { return len; }

	buffer *buf;

private:
	I chn,offs,len;

	FLEXT_CALLBACK_G(m_vasp)
	FLEXT_CALLBACK_G(m_set)

	FLEXT_CALLBACK_1(m_offset,F)
	FLEXT_CALLBACK_1(m_length,F)
	FLEXT_CALLBACK_2(m_part,F,F)
};


class vasp_dsp:
	public flext_dsp
{
	FLEXT_HEADER(vasp_dsp,flext_dsp)

protected:

};



#endif
