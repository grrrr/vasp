#include "main.h"

class vasp_prepend:
	public flext_base
{
	FLEXT_HEADER(vasp_prepend,flext_base)

public:
	vasp_prepend(I argc,t_atom *argv);

	virtual V m_set(I argc,t_atom *argv);

private:
	virtual V m_method_(I inlet,t_symbol *s,I argc,t_atom *argv);

	FLEXT_CALLBACK_G(m_set)
};

FLEXT_NEW_G("vasp.prepend",vasp_prepend)


vasp_prepend::vasp_prepend(I argc,t_atom *argv)
{
	FLEXT_ADDMETHOD_(0,"set",m_set);
}

V vasp_prepend::m_set(I argc,t_atom *argv)
{
}

V vasp_prepend::m_method_(I inlet,t_symbol *s,I argc,t_atom *argv)
{
	post("%s: inlet=%i symbol=%s argc=%i",inlet,s->s_name,argc);
}


