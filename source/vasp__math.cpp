#include "main.h"

class vasp_assign:
	public vasp_msg
{
	FLEXT_HEADER(vasp_assign,vasp_msg)

public:

	virtual I m_set(I argc,t_atom *argv); // non-triggering set

	virtual I m_add(I argc,t_atom *argv); // add to (one vec or float)
	virtual I m_sub(I argc,t_atom *argv); // sub from (one vec or float)
	virtual I m_mul(I argc,t_atom *argv); // mul with (one vec or float)
	virtual I m_div(I argc,t_atom *argv); // div by (one vec or float)

	virtual I m_madd(I argc,t_atom *argv); // add to (multi-channel or float)
	virtual I m_msub(I argc,t_atom *argv); // sub from (multi-channel or float)
	virtual I m_mmul(I argc,t_atom *argv); // mul with (multi-channel or float)
	virtual I m_mdiv(I argc,t_atom *argv); // div by (multi-channel or float)

	virtual I m_min(I argc,t_atom *argv); // mul with (one vec or float)
	virtual I m_max(I argc,t_atom *argv); // div by (one vec or float)

private:
};

FLEXT_GIMME("vasp.assign",vasp_assign)


V vasp_assign::cb_setup(t_class *c)
{
	FLEXT_ADDMETHOD_G(c,"set",m_set);
	FLEXT_ADDMETHOD_G(c,"add",m_add);
	FLEXT_ADDMETHOD_G(c,"sub",m_sub);
	FLEXT_ADDMETHOD_G(c,"mul",m_mul);
	FLEXT_ADDMETHOD_G(c,"div",m_div);
}


vasp_assign::vasp_assign(I argc,t_atom *argv)
{
	m_set(argc,argv);

	add_in_def();
	add_out_anything();
	setup_inout();
}

vasp_assign::~vasp_assign()
{
}


I vasp_assign::m_set(I argc,t_atom *argv)
{
	ref(argc,argv);

	if(!ref.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}

	return 0; 
}

