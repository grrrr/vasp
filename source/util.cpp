#include "main.h"

class vasp_prepend:
	public flext_base
{
	FLEXT_HEADER(vasp_prepend,flext_base)

public:
	vasp_prepend(I argc,t_atom *argv);

	virtual V m_set(I argc,t_atom *argv);

private:
	FLEXT_CALLBACK_G(m_set)
};

FLEXT_GIMME("vasp.prepend",vasp_prepend)


V vasp_prepend::cb_setup(t_class *c)
{
	FLEXT_ADDMETHOD_G(c,"set",m_set);
}


vasp_prepend::vasp_prepend(I argc,t_atom *argv)
{
}

V vasp_prepend::m_set(I argc,t_atom *argv)
{
}



