/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file vasp_imm.cpp
	\brief Definitions for immediate vasps
*/


#include "classes.h"
#include "util.h"
#include "buflib.h"

/*! \class vasp_imm
	\remark \b vasp.imm
	\brief Get vasp immediate.
	\since 0.0.6
	\param inlet vasp - is stored and output triggered
	\param inlet bang - triggers output
	\param inlet set - vasp to be stored 
	\retval outlet vasp! - vasp immediate

*/
class vasp_imm:
	public vasp_op
{
	FLEXT_HEADER(vasp_imm,vasp_op)

public:

	vasp_imm()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual V m_bang() 
	{ 
		if(!ref.Ok())
			post("%s - Invalid vasp!",thisName());
		else if(ref.Vectors() > 1) 
			post("%s - More than one vector in vasp!",thisName());
		else {
			VBuffer *buf = ref.Buffer(0);
			I len = buf->Length(),chns = buf->Channels();

			ImmBuf *imm = BufLib::NewImm(len);

			S *dst = imm->Pointer();
			const S *src = buf->Pointer();
			for(I i = 0; i < len; ++i,src += chns,dst++) *dst = *src; 

			ToOutVasp(0,Vasp(len,Vasp::Ref(*imm)));
			delete imm;
		}
	}

	virtual V m_help() { post("%s - Get immediate vasp vectors",thisName()); }

protected:

};

FLEXT_LIB("vasp.imm vasp.!",vasp_imm)



