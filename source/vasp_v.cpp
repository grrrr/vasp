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




class vasp_split:
	public vasp_base
{
	FLEXT_HEADER(vasp_split,vasp_base)

public:

	vasp_split(FI n)
	{
		AddInAnything();
		AddOutAnything(n);
		SetupInOut();
	}

	virtual Vasp *x_work() 
	{ 
		return new Vasp(ref); 
	}
};

FLEXT_LIB_1("vasp.join",vasp_join,FI)


class vasp_join:
	public vasp_base
{
	FLEXT_HEADER(vasp_join,vasp_base)

public:

	vasp_join(FI n)
	{
		AddInAnything(n);
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() 
	{ 
		return new Vasp(ref); 
	}
};

FLEXT_LIB_1("vasp.join",vasp_join,FI)


class vasp_offs:
	public vasp_base
{
	FLEXT_HEADER(vasp_offs,vasp_base)

public:

	vasp_offs(I argc,t_atom *argv)
	{
		AddInAnything();
		AddInFlint();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() 
	{ 
		return new Vasp(ref); 
	}
};

FLEXT_LIB_G("vasp.offs",vasp_offs)


class vasp_len:
	public vasp_base
{
	FLEXT_HEADER(vasp_len,vasp_base)

public:

	vasp_len(I argc,t_atom *argv)
	{
		AddInAnything();
		AddInFlint();
		AddOutAnything();
		SetupInOut();
	}

	virtual Vasp *x_work() 
	{ 
		return new Vasp(ref); 
	}
};

FLEXT_LIB_G("vasp.len",vasp_len)

