/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

///////////////////////////////////////////////////////////////////////////
// vasp_tx class
///////////////////////////////////////////////////////////////////////////


vasp_tx::vasp_tx(I argc,t_atom *argv)
{
	m_set(argc,argv);

	AddInAnything(2);
	AddOutAnything();
	SetupInOut();

	FLEXT_ADDMETHOD(1,a_list);
	FLEXT_ADDMETHOD_(1,"vasp",a_vasp);
	FLEXT_ADDMETHOD_(1,"float",a_float);
	FLEXT_ADDMETHOD_(1,"complex",a_complex);
	FLEXT_ADDMETHOD_(1,"vector",a_vector);
}

vasp_tx::~vasp_tx() {}


V vasp_tx::a_list(I argc,t_atom *argv) 
{ 
	if(argc) {
		arg.Parse(argc,argv);
		if(arg.IsNone()) 
			post("%s - list argument could not be evaluated (ignored)",thisName());
	}
	else {
		post("%s - Empty list argument (ignored)",thisName());
	}
}

V vasp_tx::a_vasp(I argc,t_atom *argv) 
{ 
	Vasp *v = new Vasp(argc,argv);
	if(v->Ok()) 
		arg.Set(v);
	else {
		post("%s - invalid argument vasp (ignored)",thisName());
		delete v;
	}
}

V vasp_tx::a_float(F v) { arg.Set(v); }

V vasp_tx::a_complex(I argc,t_atom *argv) 
{ 
	if(
		(argc == 1 && IsFloat(argv[0])) || 
		(argc == 2 && IsFloat(argv[0]) && IsFloat(argv[1]))
	) {
		arg.Set(GetAFloat(argv[0]),GetAFloat(argv[1]));
	}
	else 
		post("%s - invalid complex argument (ignored)",thisName());
}

V vasp_tx::a_vector(I argc,t_atom *argv)
{
	error("%s - vector type not implemented",thisName());
}


Vasp *vasp_tx::x_work() { return tx_work(arg); }

Vasp *vasp_tx::tx_work(const Argument &arg) 
{
	error("%s - no work method implemented",thisName());
	return NULL;
}

