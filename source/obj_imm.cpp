/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file vasp_v.cpp
	\brief Definitions of basic vasp objects.
*/


#include "classes.h"
#include "util.h"


/*! \class vasp_i
	\remark \b vasp.!
	\brief Get vasp immediate.
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet vasp! - vasp immediate

*/
class vasp_i:
	public vasp_op
{
	FLEXT_HEADER(vasp_i,vasp_op)

public:

	vasp_i()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		post("%s - Sorry, not implemented yet",thisName());
	}

	virtual V m_help() { post("%s - Get immediate vasp vectors",thisName()); }
};

FLEXT_LIB("vasp.!",vasp_i)



