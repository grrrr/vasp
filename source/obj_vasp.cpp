/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file obj_vasp.cpp
	\brief basic vasp objects.
*/


#include "classes.h"
#include "util.h"

/*! \class vasp_v 
	\remark \b vasp
	\brief Stores vasp messages.
	\since 0.0.1
	\param cmdln.* vasp - to be stored
	\param inlet.1 vasp - is stored and output
	\param inlet.1 list - if possible list is converted to vasp format
	\param inlet.1 bang - triggers stored Vasp output
	\param inlet.1 set vasp - Vasp is stored (and not immediately output)
	\param inlet.2 vasp - Vasp is stored (and not immediately output)
	\retval outlet vasp
*/
class vasp_v:
	public vasp_tx
{
	FLEXT_HEADER(vasp_v,vasp_tx)

public:

	vasp_v(I argc,t_atom *argv)
	{
		m_set(argc,argv);

		AddInAnything(2);
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"list",m_vasp);

		FLEXT_ADDMETHOD_(1,"list",m_set);
		FLEXT_ADDMETHOD_(1,"vasp",m_set);
	}

	virtual Vasp *x_work() { return new Vasp(ref); }

	virtual V m_help() { post("%s - Store and output a vasp",thisName()); }
};

FLEXT_LIB_V("vasp",vasp_v)



/*! \class vasp_update
	\remark \b vasp.update
	\brief Refreshes buffer graphics for a vasp.
	\since 0.0.1
	\param inlet vasp - is stored and output
	\param inlet bang - triggers stored vasp output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet vasp

	\note In MaxMSP only necessary when buffer is in another window.
*/
class vasp_update:
	public vasp_tx
{
	FLEXT_HEADER(vasp_update,vasp_tx)

public:
	vasp_update()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() { ref.Refresh(); return new Vasp(ref); }

	virtual V m_help() { post("%s - Update graphics of a vasp",thisName()); }
};

FLEXT_LIB("vasp.update vasp.u",vasp_update)



/*! \class vasp_check
	\remark \b vasp.check
	\brief Check vasp dimensions.
	\since 0.0.1
	\param inlet vasp - is stored and output
	\param inlet bang - triggers stored vasp output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet vasp

	\remark checks and corrects frame count
	\remark checks channel index... no correction, no output on error!
*/
class vasp_check:
	public vasp_tx
{
	FLEXT_HEADER(vasp_check,vasp_tx)

public:
	vasp_check()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		I fr = ret->ChkFrames(); // maximum common frame length
		ret->Frames(fr);

		BL chok = true;

		for(I i = 0; i < ret->Vectors(); ++i) {
			VBuffer *buf = ret->Buffer(i);
			chok = chok && buf->Ok() && buf->Channel() == ret->Vector(i).Channel();
			delete buf;
		}

		if(chok) 
			return ret;
		else {
			delete ret;
			return NULL;
		}
	}

	virtual V m_help() { post("%s - Check vasp dimensions",thisName()); }
};

FLEXT_LIB("vasp.check vasp.chk",vasp_check)



/*! \class vasp_m 
	\remark \b vasp.m
	\brief Outputs multiple (identical) vasps.
	\since 0.0.1
	\param cmdln.1 int - number of vasp outlets
	\param inlet vasp - is stored and output
	\param inlet bang - triggers stored Vasp output
	\param inlet set - vasp to be stored (and not immediately output)
	\retval outlet.* vasp

	\note Outputs in right to left order.
*/
class vasp_m:
	public vasp_op
{
	FLEXT_HEADER(vasp_m,vasp_op)

public:

	vasp_m(I n)
	{
		AddInAnything();
		AddOutAnything(n);
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		Vasp r(ref); 
		for(I i = CntOut()-1; i >= 0; --i) ToOutVasp(i,r);
	}

	virtual V m_help() { post("%s - Output a vasp multiply",thisName()); }
};

FLEXT_LIB_1("vasp.m",vasp_m,I)



