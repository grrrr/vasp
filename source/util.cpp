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


vasp_prepend::vasp_prepend(I argc,t_atom *argv)
{
	FLEXT_ADDMETHOD_(0,"set",m_set);
}

V vasp_prepend::m_set(I argc,t_atom *argv)
{
}



