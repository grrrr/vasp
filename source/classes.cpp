/* 

VASP modular - vector assembling signal processor / objects for Max/MSP and PD

Copyright (c) 2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "classes.h"
//#include <stdarg.h>


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


///////////////////////////////////////////////////////////////////////////
// vasp_unop class
///////////////////////////////////////////////////////////////////////////

vasp_unop::vasp_unop(BL op,UL outcode):
	vasp_tx(op)
{
	AddInAnything();
	AddOutAnything(1);
	AddOutlets(outcode);
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


vasp_binop::vasp_binop(I argc,t_atom *argv,BL op,UL outcode):
	vasp_tx(op)
{
	a_list(argc,argv);

	AddInAnything(2);
	AddOutAnything(1);
	AddOutlets(outcode);
	SetupInOut();

	FLEXT_ADDMETHOD(1,a_list);
	FLEXT_ADDMETHOD_(1,"vasp",a_vasp);
	FLEXT_ADDMETHOD_(1,"float",a_float);
	FLEXT_ADDMETHOD_(1,"int",a_int);
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

V vasp_binop::a_int(I v) { arg.Set(v); }

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


vasp_anyop::vasp_anyop(I argc,t_atom *argv,BL op,UL outcode):
	vasp_tx(op)
{
	a_list(argc,argv);

	AddInAnything(2);
	AddOutAnything(1);
	AddOutlets(outcode);

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

