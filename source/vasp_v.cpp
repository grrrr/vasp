#include "main.h"

class vasp_v:
	public vasp_base
{
	FLEXT_HEADER(vasp_v,vasp_base)

public:

	vasp_v(I argc,t_atom *argv)
	{
		m_set(argc,argv);

		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() { return new Vasp(ref); }
};

FLEXT_LIB_G("vasp",vasp_v)




class vasp_update:
	public vasp_base
{
	FLEXT_HEADER(vasp_update,vasp_base)

public:

	vasp_update()
	{
		AddInAnything();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() { ref.Refresh(); return new Vasp(ref); }
};

FLEXT_LIB("vasp.update",vasp_update)

