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


vasp_tx::vasp_tx(I argc,t_atom *argv):
	argtp(tp_none)
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

vasp_tx::~vasp_tx()
{
	ClearArg();
}


V vasp_tx::a_list(I argc,t_atom *argv) 
{ 
//	post("%s - vasp method called",thisName());

	SetArg(argc,argv);
}

V vasp_tx::a_vasp(I argc,t_atom *argv) 
{ 
//	post("%s - vasp method called",thisName());

	vasp *v = new vasp(argc,argv);
	if(v->Ok()) 
		SetArg(v);
	else {
		post("%s - invalid argument vasp (ignored)",thisName());
		delete v;
	}
}

V vasp_tx::a_float(F v) 
{ 
//	post("%s - float method called",thisName());

	SetArg(v);
}

V vasp_tx::a_complex(I argc,t_atom *argv) 
{ 
//	post("%s - complex method called",thisName());

	if(
		(argc == 1 && IsFloat(argv[0])) || 
		(argc == 2 && IsFloat(argv[0]) && IsFloat(argv[1]))
	) {
		SetArg(GetAFloat(argv[0]),GetAFloat(argv[1]));
	}
	else 
		post("%s - invalid complex argument (ignored)",thisName());
}

V vasp_tx::a_vector(I argc,t_atom *argv)
{
	error("%s - vector type not implemented",thisName());
}


vasp *vasp_tx::x_work() 
{
	switch(argtp) {
	case tp_vasp:
		ASSERT(arg.v != NULL);
		return tx_vasp(*arg.v);
		break;
	case tp_list:
		ASSERT(arg.atoms != NULL);
		return tx_list(arg.atoms->Count(),arg.atoms->Atoms());
		break;
	case tp_float:
		return tx_float(arg.f);
		break;
	case tp_cx:
		return tx_complex(arg.cx.real,arg.cx.imag);
		break;
	case tp_vx:
		ASSERT(arg.vx != NULL);
		return tx_vector(*arg.vx);
		break;
	}
	return NULL;
}


V vasp_tx::ClearArg()
{
	switch(argtp) {
	case tp_vasp:
		if(arg.v) { delete arg.v; arg.v = NULL; }
		argtp = tp_none;
		break;
	case tp_list:
		if(arg.atoms) { delete arg.atoms; arg.atoms = NULL; }
		argtp = tp_none;
		break;
	case tp_vx:
		if(arg.vx) { delete arg.vx; arg.vx = NULL; }
		argtp = tp_none;
		break;
	case tp_float:
	case tp_cx:
		argtp = tp_none;
		break;
	}
}

V vasp_tx::SetArg(vasp *v)
{
	if(argtp != tp_none) ClearArg();
	arg.v = v; argtp = tp_vasp;
}

V vasp_tx::SetArg(I argc,t_atom *argv)
{
	if(argtp != tp_none) ClearArg();
	arg.atoms = new AtomList(argc,argv); argtp = tp_list;
}

V vasp_tx::SetArg(F f)
{
	if(argtp != tp_none) ClearArg();
	arg.f = f; argtp = tp_float;
}

V vasp_tx::SetArg(F re,F im)
{
	if(argtp != tp_none) ClearArg();
	arg.cx.real = re; arg.cx.imag = im; argtp = tp_cx;
}

V vasp_tx::SetArg(VX *vec)
{
	if(argtp != tp_none) ClearArg();
	arg.vx = vec; argtp = tp_vx;
}


vasp *vasp_tx::tx_vasp(const vasp &v)
{
	error("%s - vasp type not implemented",thisName());
	return NULL;
}

vasp *vasp_tx::tx_list(I argc,const t_atom *argv)
{
	error("%s - list type not implemented",thisName());
	return NULL;
}

vasp *vasp_tx::tx_float(F f)
{
	error("%s - float type not implemented",thisName());
	return NULL;
}

vasp *vasp_tx::tx_complex(F re,F im)
{
	error("%s - complex type not implemented",thisName());
	return NULL;
}

vasp *vasp_tx::tx_vector(const VX &v)
{
	error("%s - vector type not implemented",thisName());
	return NULL;
}


