/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file obj_radio.cpp
	\brief objects for radio messages.
*/


#include "classes.h"

/*! \class vasp_radio
	\remark \b vasp.radio
	\brief Lets only radio messages pass through.
	\since 0.0.6
	\param inlet.1 * - any message
	\retval outlet.1 radio messages
	\retval outlet.2 other messages
*/
class vasp_radio:
	public flext_base
{
	FLEXT_HEADER(vasp_radio,flext_base)

public:

	vasp_radio()
	{
		AddInAnything();
		AddOutAnything(2);
		SetupInOut();

		FLEXT_ADDMETHOD(0,m_any);
	}

	virtual V m_any(const t_symbol *s,I argc,t_atom *argv);

	virtual V m_help() { post("%s - split into radio and non-radio messages",thisName()); }
private:
	FLEXT_CALLBACK_A(m_any);
};

FLEXT_LIB("vasp, vasp.radio",vasp_radio)


V vasp_radio::m_any(const t_symbol *s,I argc,t_atom *argv) 
{
	ToOutAnything(s == vasp_base::sym_radio?0:1,s,argc,argv);
}



/*! \class vasp_noradio
	\remark \b vasp.!radio
	\brief Filters out radio messages 
	\since 0.0.6
	\param inlet.1 * - any message
	\retval outlet all messages except radio messages
*/
class vasp_noradio:
	public vasp_radio
{
	FLEXT_HEADER(vasp_noradio,vasp_radio)

public:

	virtual V m_any(t_symbol *s,I argc,t_atom *argv);

	virtual V m_help() { post("%s - filters out all radio messages",thisName()); }
};

FLEXT_LIB("vasp, vasp.noradio vasp.!radio",vasp_noradio)


V vasp_noradio::m_any(t_symbol *s,I argc,t_atom *argv) 
{
	if(s != vasp_base::sym_radio) ToOutAnything(0,s,argc,argv);
}
