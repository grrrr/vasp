#include "main.h"

V vasp_modify::m_copy(I argc,t_atom *argv)
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
		return;
	}

}

V vasp_modify::m_mcopy(I argc,t_atom *argv)
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
		return;
	}

}





V vasp_modify::m_add(I argc,t_atom *argv)
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
		return;
	}

}

V vasp_modify::m_madd(I argc,t_atom *argv)
{
	vasp arg(argc,argv);
	if(!arg.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
		return;
	}

}


