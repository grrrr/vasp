/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "classes.h"
#include "util.h"


/*! \class vasp_sync
	\remark \b vasp.sync
	\brief Waits for all inlets to be hit (by vasps/anything) to trigger output.
	\since 0.0.1
	\param cmdln.1 int - number of sync inlets
	\param inlet.1 vasp - is stored 
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
	\param inlet.1 reset - clear all hit flags
	\param inlet.+n vasp/anything - sets hit flag
	\retval outlet vasp - stored vasp 

	\todo Message for selection if only vasp input triggers (or any one).
	\todo Message for selection of manual or auto reset upon trigger
*/
class vasp_sync:
	public vasp_op
{
	FLEXT_HEADER(vasp_sync,vasp_op)

public:
	vasp_sync(I n):
		flags(new BL[n]),
		autoreset(true),vasponly(false)
	{
		AddInAnything(n);
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD_(0,"reset",m_reset);

		m_reset();
	}

	~vasp_sync()	{ if(flags) delete[] flags; }

	V chkbang(I n) 
	{
		BL f = flags[n]; 
		flags[n] = true; 
		if(!f) { // flags have changed
			
			BL all = true;
			for(I i = 0; i < CntIn(); ++i) all = all && flags[i];

			if(all) {
				if(ref.Ok()) ToOutVasp(0,ref);
				else ToOutBang(0);

				if(autoreset) m_reset();
			}
		}
	}

	virtual V m_bang() { chkbang(0); }

	V m_reset() 
	{ 
		for(I i = 0; i < CntIn(); ++i) flags[i] = false;
	}

	virtual bool m_method_(I inlet,const t_symbol *s,I argc,t_atom *argv)
	{
		if(inlet > 0 && (!vasponly || s == sym_vasp)) {
			chkbang(inlet);
			return true;
		}
		else
			return vasp_op::m_method_(inlet,s,argc,argv);
	}

	virtual V m_help() { post("%s - Synchronize a number of vasps",thisName()); }
private:
	BL autoreset,vasponly;
	BL *flags;

	FLEXT_CALLBACK(m_reset)
};

FLEXT_LIB_1("vasp.sync",vasp_sync,I)



