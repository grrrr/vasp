/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "classes.h"
#include "util.h"


/*! \class vasp_channel
	\remark \b vasp.channel
	\brief Sets channel index of vasp.
	\since 0.0.8
	\param cmdln.1 int - index of vasp vector
	\param inlet vasp - is stored and indexed vasp vector output
	\param inlet bang - triggers indexed vasp vector output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet modified vasp
*/
class vasp_channel:
	public vasp_tx
{
	FLEXT_HEADER(vasp_channel,vasp_tx)

public:
	vasp_channel(I argc,t_atom *argv):
		ix(0)
	{
		if(argc >= 1 && CanbeInt(argv[0]))
			ix = GetAInt(argv[0]);
		else if(argc)
			post("%s - Index argument invalid -> set to 0",thisName());

		AddInAnything(2);
		AddOutAnything();

		FLEXT_ADDMETHOD(1,m_ix);
		FLEXT_ADDATTR_VAR("index",ix,m_ix);
	}

	V m_ix(I i) { ix = i; }

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref);
		ret->Channel(ix);
		return ret; 
	}

	virtual V m_help() { post("%s - Set channel index of vectors in vasp",thisName()); }

protected:
	I ix;

private:
	FLEXT_CALLBACK_I(m_ix);
	FLEXT_CALLSET_I(m_ix);
	FLEXT_ATTRGET_I(ix);
};

FLEXT_LIB_V("vasp, vasp.channel vasp.c",vasp_channel)



/*! \class vasp_qc 
	\remark \b vasp.c?
	\brief Gets channel index of a vasp.
	\since 0.0.8
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet int - channel index of stored vasp

	\note Always returns index of 0th vasp
	\note No output for invalid vasp?
*/
class vasp_qchannel:
	public vasp_op
{
	FLEXT_HEADER(vasp_qchannel,vasp_op)

public:
	vasp_qchannel()
	{
		AddInAnything();
		AddOutInt();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		if(ref.Ok()) 
			ToOutInt(0,ref.Vector(0).Channel()); 
		else
			post("%s - Invalid vasp, no output",thisName());
	}

	virtual V m_help() { post("%s - Get channel index of 0th vector in vasp",thisName()); }
};

FLEXT_LIB("vasp, vasp.channel? vasp.c?",vasp_qchannel)



