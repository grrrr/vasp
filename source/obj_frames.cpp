/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "classes.h"
#include "util.h"

/*! \class vasp_frames
	\remark \b vasp.frames
	\brief Sets frame count of vasp.
	\since 0.0.1
	\param cmdln.1 [_time=0] - frame count in time units
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 _time - frame count in time units
	\retval outlet vasp - modified vasp

	\todo Implement unit processing.
*/
class vasp_frames:
	public vasp_tx
{
	FLEXT_HEADER(vasp_frames,vasp_tx)

public:
	vasp_frames(I argc,t_atom *argv):
		frms(0),setf(false)
	{
		if(argc >= 1 && CanbeInt(argv[0])) 
			m_frms(GetAInt(argv[0]));
		else if(argc)
			post("%s - Frame count argument invalid -> ignored",thisName());

		AddInAnything();
		AddInFloat();
		AddOutAnything();
		SetupInOut();

		FLEXT_ADDMETHOD(1,m_frms);
	}

	V m_frms(F f) 
	{ 
		frms = f; //! \todo unit processing
		setf = true; 
	}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(setf) ret->Frames(frms);
		return ret;
	}

	virtual V m_help() { post("%s - Set a vasp's frame count",thisName()); }
protected:
	I frms;
	BL setf;

private:
	FLEXT_CALLBACK_F(m_frms);
};

FLEXT_LIB_G("vasp.frames",vasp_frames)




/*! \class vasp_dframes
	\remark \b vasp.frames+
	\brief Sets frame count of vasp differentially.
	\since 0.0.1
	\param cmdln.1 [_time=0] - increase of frame count in time units
	\param inlet.1 vasp - is stored and output triggered
	\param inlet.1 bang - triggers output
	\param inlet.1 set - vasp to be stored 
    \param inlet.2 _time - increase of frame count in time units
	\retval outlet vasp - modified vasp

	\todo Implement unit processing.
*/
class vasp_dframes:
	public vasp_frames
{
	FLEXT_HEADER(vasp_dframes,vasp_frames)

public:
	vasp_dframes(I argc,t_atom *argv): vasp_frames(argc,argv) {}

	virtual Vasp *x_work() 
	{ 
		Vasp *ret = new Vasp(ref); 
		if(setf) ret->FramesD(frms);
		return ret;
	}

	virtual V m_help() { post("%s - Raise/lower a vasp's frame count",thisName()); }
};

FLEXT_LIB_G("vasp.frames+",vasp_dframes)



/*! \class vasp_qframes
	\remark \b vasp.frames?
	\brief Get frame count in time units
	\since 0.0.1
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet _time - frame count of vasp in time units

	\note Outputs 0 if vasp is undefined or invalid

	\todo Implement unit processing
	\todo Should we provide a cmdln default vasp?
	\todo Should we inhibit output for invalid vasps?
*/
class vasp_qframes:
	public vasp_op
{
	FLEXT_HEADER(vasp_qframes,vasp_op)

public:

	vasp_qframes()
	{
		AddInAnything();
		AddOutInt();
		SetupInOut();
	}

	virtual V m_bang() { ToOutInt(0,ref.ChkFrames()); }	//! \todo unit processing

	virtual V m_help() { post("%s - Get a vasp's frame count",thisName()); }
};

FLEXT_LIB("vasp.frames?",vasp_qframes)



