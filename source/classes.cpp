/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"


///////////////////////////////////////////////////////////////////////////
// vasp_unop class
///////////////////////////////////////////////////////////////////////////

vasp_unop::vasp_unop(BL op,I outs):
	vasp_tx(op)
{
	AddInAnything();
	AddOutAnything(1+outs);
	SetupInOut();
}

Vasp *vasp_unop::x_work() { return tx_work(); }

Vasp *vasp_unop::tx_work() 
{
	error("%s - no work method implemented",thisName());
	return NULL;
}

///////////////////////////////////////////////////////////////////////////
// vasp_binop class
///////////////////////////////////////////////////////////////////////////


vasp_binop::vasp_binop(I argc,t_atom *argv,BL op,I outs):
	vasp_tx(op)
{
	a_list(argc,argv);

	AddInAnything(2);
	AddOutAnything(1+outs);
	SetupInOut();

	FLEXT_ADDMETHOD(1,a_list);
	FLEXT_ADDMETHOD_(1,"vasp",a_vasp);
	FLEXT_ADDMETHOD_(1,"float",a_float);
	FLEXT_ADDMETHOD_(1,"complex",a_complex);
	FLEXT_ADDMETHOD_(1,"vector",a_vector);
}

V vasp_binop::a_list(I argc,t_atom *argv) 
{ 
	if(argc) {
		arg.Parse(argc,argv);
		if(arg.IsNone()) 
			post("%s - list argument could not be evaluated (ignored)",thisName());
		else if(argchk) {
			// check argument feasibility
		}
	}
	else {
//		post("%s - Empty list argument (ignored)",thisName());
	}
}

V vasp_binop::a_vasp(I argc,t_atom *argv) 
{ 
	Vasp *v = new Vasp(argc,argv);
	if(v->Ok()) {
		arg.Set(v);
		if(argchk) {
			// check argument feasibility
		}
	}
	else {
		post("%s - invalid argument vasp (ignored)",thisName());
		delete v;
	}
}

V vasp_binop::a_float(F v) { arg.Set(v); }

V vasp_binop::a_complex(I argc,t_atom *argv) 
{ 
	if(
		(argc == 1 && IsFloat(argv[0])) || 
		(argc == 2 && IsFloat(argv[0]) && IsFloat(argv[1]))
	) {
		arg.Set(GetAFloat(argv[0]),GetAFloat(argv[1]));
		if(argchk) {
			// check argument feasibility
		}
	}
	else 
		post("%s - invalid complex argument (ignored)",thisName());
}

V vasp_binop::a_vector(I argc,t_atom *argv)
{
	error("%s - vector type not implemented",thisName());
}


Vasp *vasp_binop::x_work() { return tx_work(arg); }

Vasp *vasp_binop::tx_work(const Argument &arg) 
{
	error("%s - no work method implemented",thisName());
	return NULL;
}


///////////////////////////////////////////////////////////////////////////
// vasp_anyop class
///////////////////////////////////////////////////////////////////////////


vasp_anyop::vasp_anyop(I argc,t_atom *argv,BL op,I outs):
	vasp_tx(op)
{
	a_list(argc,argv);

	AddInAnything(2);
	AddOutAnything(1+outs);
	SetupInOut();

	FLEXT_ADDMETHOD(1,a_list);
}

V vasp_anyop::a_list(I argc,t_atom *argv) 
{ 
	if(argc) {
		arg.Set(argc,argv);
		if(arg.IsNone()) 
			post("%s - argument could not be evaluated (ignored)",thisName());
		else if(argchk) {
			// check argument feasibility
		}
	}
	else {
//		post("%s - Empty list argument (ignored)",thisName());
	}
}

Vasp *vasp_anyop::x_work() { return tx_work(arg); }

Vasp *vasp_anyop::tx_work(const Argument &arg) 
{
	error("%s - no work method implemented",thisName());
	return NULL;
}

