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

V vasp_base::setup(t_class *c)
{
	sym_radio = gensym("radio");
	sym_vasp = gensym("vasp");
}

vasp_base::vasp_base()
{
	FLEXT_ADDBANG(0,m_bang);
	FLEXT_ADDMETHOD_(0,"vasp",m_vasp);
	FLEXT_ADDMETHOD_(0,"set",m_set);

	FLEXT_ADDMETHOD_(0,"radio",m_radio);

	FLEXT_ADDMETHOD_E(0,"update",m_update);
	FLEXT_ADDMETHOD_E(0,"unit",m_unit);
}

vasp_base::~vasp_base()
{
}

I vasp_base::m_set(I argc,t_atom *argv)
{
	vasp arg(argc,argv);

	if(argc && !arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}
	else 
		ref = arg;

	return 0; 
}

V vasp_base::m_bang()
{
	x_work();
	if(ref.Ok()) {
		ref.MakeList(false);
		ToOutAnything(0,sym_vasp,ref.Atoms(),ref.AtomList());
	}
}

V vasp_base::m_vasp(I argc,t_atom *argv)
{
	m_set(argc,argv);
	m_bang();
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


V vasp_base::m_unit(xs_unit u) {	unit = u; }

V vasp_base::m_update(I argc,t_atom *argv) 
{
	if(argc == 0) {
		for(I i = 0; i < ref.Vectors(); ++i) {
			vbuffer *vb = ref.Buffer(i);
			if(vb) {
				vb->Dirty(true);
				delete vb;
			}
		}
	}
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


vasp_tx::vasp_tx(I argc,t_atom *argv):
	argtp(at_none)
{
	m_set(argc,argv);

	AddInAnything(2);
	AddOutAnything();
	SetupInOut();

	FLEXT_ADDMETHOD_(1,"vasp",a_vasp);
	FLEXT_ADDMETHOD_(1,"float",a_float);
	FLEXT_ADDMETHOD_(1,"complex",a_complex);
	FLEXT_ADDMETHOD_(1,"vector",a_vector);
}

vasp_tx::~vasp_tx()
{
}

V vasp_tx::a_vasp(I argc,t_atom *argv) 
{ 
//	post("%s - vasp method called",thisName());

	argtp = at_vasp;
	arg_V(argc,argv);
}

V vasp_tx::a_float(F v) 
{ 
//	post("%s - float method called",thisName());

	argtp = at_float;
	arg_F = v;
}

V vasp_tx::a_complex(I argc,t_atom *argv) 
{ 
//	post("%s - complex method called",thisName());

	if(
		(argc == 1 && IsFloat(argv[0])) || 
		(argc == 2 && IsFloat(argv[0]) && IsFloat(argv[1]))
	) {
		argtp = at_complex;
		
		arg_CX.real = GetAFloat(argv[0]);
		arg_CX.imag = GetAFloat(argv[1]);
	}
	else 
		post("%s - complex argument is invalid (-> ignored)",thisName());
}

V vasp_tx::a_vector(I argc,t_atom *argv)
{
	post("%s - vector type not implemented",thisName());
}


V vasp_tx::x_work() 
{
//	post("%s: supposed to do work",thisName());

	switch(argtp) {
		case at_none: tx_none(); break;
		case at_vasp: tx_vasp(arg_V); break;
		case at_float: tx_float(arg_F); break;
		case at_complex: tx_complex(arg_CX); break;
		case at_vector:	tx_vector(arg_VX); break;
		default:
			post("%s - argument type not handled",thisName());
	}
}

V vasp_tx::tx_none() {}
V vasp_tx::tx_vasp(const vasp &v) { post("%s - vasp argument is not handled",thisName()); }
V vasp_tx::tx_float(F v) { post("%s - float argument is not handled",thisName()); }
V vasp_tx::tx_complex(const CX &v) { post("%s - complex argument is not handled",thisName()); }
V vasp_tx::tx_vector(const VX &v) { post("%s - vector argument is not handled",thisName()); }
