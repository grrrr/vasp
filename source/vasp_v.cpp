#include "main.h"

class vasp_v:
	public vasp_msg
{
	FLEXT_HEADER(vasp_v,vasp_msg)

public:

	vasp_v(I argc,t_atom *argv);
	~vasp_v();

	virtual V m_vasp(I argc,t_atom *argv); // trigger

	virtual I m_set(I argc,t_atom *argv); // non-triggering set

protected:

private:

};

FLEXT_GIMME("vasp.v",vasp_v)


V vasp_v::cb_setup(t_class *c)
{
}


vasp_v::vasp_v(I argc,t_atom *argv)
{
	m_set(argc,argv);
}

vasp_v::~vasp_v()
{
}


V vasp_v::m_vasp(I argc,t_atom *argv)
{
	m_set(argc,argv);


	to_out_list();
}

I vasp_v::m_set(I argc,t_atom *argv)
{
	vasp_msg::m_set(argc,argv);
	return 0;
}

