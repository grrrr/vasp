#include "main.h"

class vasp_v:
	public vasp_msg
{
	FLEXT_HEADER(vasp_v,vasp_msg)

public:

	vasp_v(I argc,t_atom *argv);
	~vasp_v();

	virtual I m_set(I argc,t_atom *argv); // non-triggering set

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

FLEXT_GIMME("vasp.v",vasp_v)


V vasp_v::cb_setup(t_class *c)
{
/*
	FLEXT_ADDMETHOD_1(c,"offset",m_offset,F);
	FLEXT_ADDMETHOD_1(c,"length",m_length,F);
	FLEXT_ADDMETHOD_2(c,"part",m_part,F,F);
*/
}


vasp_v::vasp_v(I argc,t_atom *argv)
	//:buf(NULL)
{
	m_set(argc,argv);

	add_in_def();
	add_out_anything();
	setup_inout();
}

vasp_v::~vasp_v()
{
	//if(buf) delete buf;
}


I vasp_v::m_set(I argc,t_atom *argv)
{
	ref(argc,argv);

	if(!ref.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}

	return 0; //Set(v.Length(),v.Sym(),v.Channel(),v.Offset());
}

