/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

class vasp_copy:
	public vasp_tx
{
	FLEXT_HEADER(vasp_copy,vasp_tx)

public:
	vasp_copy(I argc,t_atom *argv);

protected:
	virtual V tx_list(I argc,t_atom *argv);
/*
	virtual V tx_vasp(const vasp &v);
	virtual V tx_float(F v);
	virtual V tx_complex(const CX &v);
	virtual V tx_vector(const VX &v);
*/
};


FLEXT_NEW_G("vasp.=",vasp_copy)

vasp_copy::vasp_copy(I argc,t_atom *argv):
	vasp_tx(argc,argv)
{
//	post("%s - copy setup called",thisName());
}

V vasp_copy::tx_list(I argc,t_atom *argv)
{
	post("%s - work with list",thisName());

}

/*
V vasp_copy::tx_vasp(const vasp &v)
{
	post("%s - work with vasp",thisName());

}

V vasp_copy::tx_float(F v)
{
	post("%s - work with float",thisName());
}

V vasp_copy::tx_complex(const CX &v) {}
V vasp_copy::tx_vector(const VX &v) {}
*/
