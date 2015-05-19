/*

VASP modular - vector assembling signal processor / objects for Max and Pure Data

Copyright (c)2002-2015 Thomas Grill (gr@grrrr.org)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

#include "main.h"
#include "classes.h"

///////////////////////////////////////////////////////////////////////////
// vasp_base class
///////////////////////////////////////////////////////////////////////////

const t_symbol *vasp_base::sym_radio;
const t_symbol *vasp_base::sym_vasp;
const t_symbol *vasp_base::sym_env;
const t_symbol *vasp_base::sym_double;
const t_symbol *vasp_base::sym_complex;
const t_symbol *vasp_base::sym_vector;

V vasp_base::Setup(t_classid c)
{
	sym_radio = MakeSymbol("radio");
	sym_vasp = MakeSymbol("vasp");
	sym_env = MakeSymbol("env");
	sym_double = MakeSymbol("double");
	sym_complex = MakeSymbol("complex");
	sym_vector = MakeSymbol("vector");

	FLEXT_CADDMETHOD_(c,0,"radio",m_radio);
	FLEXT_CADDMETHOD_(c,0,"help",m_help);

	FLEXT_CADDATTR_VAR1(c,"defer",deferred);

// LATER!
/*
	FLEXT_CADDATTR_VAR1(c,"argchk",argchk);
	FLEXT_CADDATTR_VAR1(c,"loglvl",loglvl);
	FLEXT_CADDATTR_VAR1_E(c,"unit",unit);
*/
}

vasp_base::vasp_base():
	refresh(false),argchk(false),deferred(false),
	unit(xsu_sample),loglvl(0)
{}

vasp_base::~vasp_base() {}


V vasp_base::m_radio(I argc,const t_atom *argv)
{
	if(argc > 0 && IsSymbol(argv[0])) {
		// send command to self!
		ToSelfAnything(0,GetSymbol(argv[0]),argc-1,argv+1);

		// send command to the next objects in line
		ToOutAnything(0,sym_radio,argc,argv);
	}
	else 
		post("%s - radio message invalid",thisName());
}

/*
V vasp_base::m_unit(xs_unit u) { unit = u; }
V vasp_base::m_argchk(BL chk) {	argchk = chk; }
V vasp_base::m_loglvl(I lvl) { loglvl = lvl; }
*/

BL vasp_base::ToOutVasp(I oix,Vasp &v) 
{
	AtomList *lst = v.MakeList(false);
	if(lst) {
        if(deferred)
		    ToQueueAnything(oix,sym_vasp,lst->Count(),lst->Atoms());
        else
		    ToOutAnything(oix,sym_vasp,lst->Count(),lst->Atoms());
		delete lst;
		return true;
	}
	else return false;
}

