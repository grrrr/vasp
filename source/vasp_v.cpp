#include "main.h"

class vasp_v:
	public vasp_msg
{
	FLEXT_HEADER(vasp_v,vasp_msg)

public:

	vasp_v(I argc,t_atom *argv);
	~vasp_v();

	virtual I m_set(I argc,t_atom *argv); // non-triggering set

	virtual V m_length(F len);  // length in units
	virtual V m_channel(FI chn);
	virtual V m_offset(F offs);  // length in units
	virtual V m_part(F offs,F len);

/*
	I Channel() const { return chn; }
	I Offset() const { return offs; }
	I Length() const { return len; }
*/
protected:

	buffer *buf;

	virtual I Set(I len,vasp::Ref &r);

private:

	FLEXT_CALLBACK_1(m_offset,F)
	FLEXT_CALLBACK_1(m_length,F)
	FLEXT_CALLBACK_2(m_part,F,F)
};

FLEXT_GIMME("vasp.v",vasp_v)


V vasp_v::cb_setup(t_class *c)
{
	FLEXT_ADDMETHOD_1(c,"offset",m_offset,F);
	FLEXT_ADDMETHOD_1(c,"length",m_length,F);
	FLEXT_ADDMETHOD_2(c,"part",m_part,F,F);
}


vasp_v::vasp_v(I argc,t_atom *argv):
	buf(NULL)
{
	m_set(argc,argv);
}

vasp_v::~vasp_v()
{
	if(buf) delete buf;
}


I vasp_v::m_set(I argc,t_atom *argv)
{
	vasp v(argc,argv);

	if(v.Complex()) {
		post("%s - complex vasp detected: reduced to real part",thisName());
		v.MakeReal();
	}
	if(!v.Ok()) {
		post("%s - invalid vasp detected and ignored",thisName());
	}

	return 0; //Set(v.Length(),v.Sym(),v.Channel(),v.Offset());
}

