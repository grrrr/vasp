/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

///////////////////////////////////////////////////////////////////////////
// vasp_copy class
///////////////////////////////////////////////////////////////////////////

class vasp_copy:
	public vasp_tx
{
	FLEXT_HEADER(vasp_copy,vasp_tx)

public:
	vasp_copy(I argc,t_atom *argv): vasp_tx(argc,argv) {}

protected:
	virtual Vasp *tx_work(const Argument &arg) { return ref.m_copy(arg); }
};

FLEXT_NEW_G("vasp.=",vasp_copy)


///////////////////////////////////////////////////////////////////////////
// vasp_ccopy class
///////////////////////////////////////////////////////////////////////////

class vasp_ccopy:
	public vasp_tx
{
	FLEXT_HEADER(vasp_ccopy,vasp_tx)

public:
	vasp_ccopy(I argc,t_atom *argv): vasp_tx(argc,argv) {}

protected:
	virtual Vasp *tx_work(const Argument &arg) { return ref.m_ccopy(arg); }
};

FLEXT_NEW_G("vasp.c=",vasp_ccopy)


///////////////////////////////////////////////////////////////////////////
// vasp_mcopy class
///////////////////////////////////////////////////////////////////////////

class vasp_mcopy:
	public vasp_tx
{
	FLEXT_HEADER(vasp_mcopy,vasp_tx)

public:
	vasp_mcopy(I argc,t_atom *argv): vasp_tx(argc,argv) {}

protected:
	virtual Vasp *tx_work(const Argument &arg) { return ref.m_mcopy(arg); }
};

FLEXT_NEW_G("vasp.m=",vasp_mcopy)



