#include "main.h"

class vasp_v:
	public vasp_msg
{
	FLEXT_HEADER(vasp_v,vasp_msg)

public:

	vasp_v(I argc,t_atom *argv);
	~vasp_v();

protected:

	buffer *buf;

private:

};

FLEXT_GIMME("vasp.v",vasp_v)


V vasp_v::cb_setup(t_class *c)
{
}


vasp_v::vasp_v(I argc,t_atom *argv):
	buf(NULL)
{
}

vasp_v::~vasp_v()
{
	if(buf) delete buf;
}
