/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"

///////////////////////////////////////////////////////////////////////////
// vasp_base class
///////////////////////////////////////////////////////////////////////////

const t_symbol *vasp_base::sym_radio;
const t_symbol *vasp_base::sym_vasp;
const t_symbol *vasp_base::sym_complex;
const t_symbol *vasp_base::sym_vector;

V vasp_base::setup(t_class *c)
{
	sym_radio = gensym("radio");
	sym_vasp = gensym("vasp");
	sym_complex = gensym("complex");
	sym_vector = gensym("vector");
}

vasp_base::vasp_base()
{
	FLEXT_ADDMETHOD_(0,"radio",m_radio);
	FLEXT_ADDMETHOD_(0,"argchk",m_argchk);
	FLEXT_ADDMETHOD_(0,"loglvl",m_loglvl);
	FLEXT_ADDMETHOD_E(0,"unit",m_unit);
}

vasp_base::~vasp_base()
{
}


V vasp_base::m_radio(I argc,t_atom *argv)
{
	if(argc > 0 && IsSymbol(argv[0])) {
		// send command to self!
		m_methodmain(0,GetSymbol(argv[0]),argc-1,argv+1);

		// send command to the next objects in line
		ToOutAnything(0,sym_radio,argc,argv);
	}
	else 
		post("%s - radio message invalid",thisName());
}


V vasp_base::m_unit(xs_unit u) { unit = u; }

V vasp_base::m_argchk(BL chk) {	argchk = chk; }

V vasp_base::m_loglvl(I lvl) { loglvl = lvl; }


BL vasp_base::ToOutVasp(I oix,Vasp &v) 
{
	AtomList *lst = v.MakeList(false);
	if(lst) {
		ToOutAnything(oix,sym_vasp,lst->Count(),lst->Atoms());
		delete lst;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////
// vasp_op class
///////////////////////////////////////////////////////////////////////////

vasp_op::vasp_op(BL op)
{
	FLEXT_ADDBANG(0,m_bang);
	FLEXT_ADDMETHOD_(0,"vasp",m_vasp);
	FLEXT_ADDMETHOD_(0,"set",m_set);
	if(op) FLEXT_ADDMETHOD_(0,"to",m_to);

	FLEXT_ADDMETHOD_(0,"update",m_update);
}

I vasp_op::m_set(I argc,t_atom *argv)
{
	Vasp arg(argc,argv);

	if(argc && !arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}
	else 
		ref = arg;

	return 0; 
}

V vasp_op::m_vasp(I argc,t_atom *argv)
{
	m_set(argc,argv);
	m_bang();
}

V vasp_op::m_to(I argc,t_atom *argv)
{
	Vasp to(argc,argv);

	if(argc && !to.Ok()) {
		// empty vasp
		dst.Clear();
	}
	else 
		dst = to;
}

V vasp_op::m_update(I argc,t_atom *argv) 
{
	if(argc == 0) 
		ref.Refresh();
	else {
		if(IsFlint(argv[0]))
			refresh = GetAFlint(argv[0]) != 0;
		else 
			post("%s(update) - argument should be omitted or integer",thisName());
	}
}



///////////////////////////////////////////////////////////////////////////
// vasp_tx class
///////////////////////////////////////////////////////////////////////////

vasp_tx::vasp_tx(BL to): vasp_op(to) {}

V vasp_tx::m_bang()
{
//	if(ref.Ok()) 
	{
		Vasp *ret = x_work();
		if(ret) {
			AtomList *lst = ret->MakeList(false);
			if(lst) {
				ToOutAnything(0,sym_vasp,lst->Count(),lst->Atoms());
				delete lst;
			}
			else
				post("%s - empty list",thisName());
			if(refresh) ret->Refresh();
			delete ret;
		}
		else {
			post("%s - no valid return",thisName());
		}
	}
/*
	else {
		post("%s - no valid vasp to work with",thisName());
	}
*/
}

