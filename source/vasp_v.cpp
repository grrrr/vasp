#include "main.h"

class vasp_v:
	public vasp_base
{
	FLEXT_HEADER(vasp_v,vasp_base)

public:

	vasp_v(I argc,t_atom *argv);
	~vasp_v();

	virtual I m_set(I argc,t_atom *argv); // non-triggering set

	virtual V x_work() {}

/*
	virtual V m_length(F len);  // length in units
	virtual V m_channel(FI chn);
	virtual V m_offset(F offs);  // length in units
	virtual V m_part(F offs,F len);
*/
protected:

//	virtual I Set(I len,vasp::Ref &r);

private:

/*
	FLEXT_CALLBACK_1(m_offset,F)
	FLEXT_CALLBACK_1(m_length,F)
	FLEXT_CALLBACK_2(m_part,F,F)
*/
};

FLEXT_NEW_G("vasp",vasp_v)

vasp_v::vasp_v(I argc,t_atom *argv)
{
	m_set(argc,argv);

	AddInAnything();
	AddOutAnything();
	SetupInOut();

/*
	FLEXT_ADDMETHOD_1(0,"offset",m_offset,F);
	FLEXT_ADDMETHOD_1(0,"length",m_length,F);
	FLEXT_ADDMETHOD_2(0,"part",m_part,F,F);
*/
}

vasp_v::~vasp_v()
{
	//if(buf) delete buf;
}


I vasp_v::m_set(I argc,t_atom *argv)
{
	ref(argc,argv);

	if(argc && !ref.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}

	return 0; //Set(v.Length(),v.Sym(),v.Channel(),v.Offset());
}


